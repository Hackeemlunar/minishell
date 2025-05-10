/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:52:11 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/08 23:35:39 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int  collect_pipeline_cmds(t_ast *node, t_ast **out_cmds, int max)
{
    if (node->type == NODE_PIPELINE)
    {
        int left_count = collect_pipeline_cmds(node->data.bin_op_node.left, out_cmds, max);
        if (left_count < 0) return -1;
        if (left_count >= max) return -1;
        return left_count + collect_pipeline_cmds(node->data.bin_op_node.right,
                                                  out_cmds + left_count, max);
    }
    out_cmds[0] = node;
    return (1);
}

int space_or_quote(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\'' || c == '"');
}

int collect_heredoc_input(const char *delim, char *temp_file)
{
    int     fd;
    char    *line;

    fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return (perror("error in heredoc"), -1);
    while (1)
    {
        line =  readline(">");
        if (!line)
        {
            write(STDERR_FILENO, "\n", 1);
            break;
        }
        if (ft_strcmp(line, delim) == 0)
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, fd);
        free(line);
    }
    return (close(fd), 0);
}

int set_in_fds(t_in_out *io)
{
    if (io->in_mode == 1 && io->heredoc_delim)
    {
        if (collect_heredoc_input(io->heredoc_delim, "/tmp/heredoc") < 0)
            return (1);
        io->in_fd = open("/tmp/heredoc", O_RDONLY);
        if (io->in_fd < 0)
            return (perror("open"), 1);
        dup2(io->in_fd, STDIN_FILENO);
    }
    else if (io->in_mode == 0 && io->in_file)
    {
        io->in_fd = open(io->in_file, O_RDONLY);
        if (io->in_fd < 0)
            return (perror("open"), 1);
        dup2(io->in_fd, STDIN_FILENO);
    }
    else
        io->in_fd = STDIN_FILENO;
    return (0);
}

int set_out_fds(t_in_out *io)
{
    if (io->out_mode == 0 && io->out_file)
    {
        io->out_fd = open(io->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (io->out_fd < 0)
            return (perror("open"), 1);
        dup2(io->out_fd, STDOUT_FILENO);
    }
    else if (io->out_mode == 1 && io->out_file)
    {
        io->out_fd = open(io->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (io->out_fd < 0)
            return (perror("open"), 1);
        dup2(io->out_fd, STDOUT_FILENO);
    }
    else
        io->out_fd = STDOUT_FILENO;
    return (0);
}

void    add_full_path(char **argv, char **paths, t_allocs *allocs)
{
    char    full_path[512];
    int     i;

    if (!paths || !*paths)
        return ;
    if (argv[0][0] == '/' && access(argv[0], F_OK) == 0)
        return ;
    ft_memset(full_path, 0, sizeof(full_path));
    i = 0;
    while (paths[i])
    {
        ft_strlcpy(full_path, paths[i], sizeof(full_path));
        ft_strlcat(full_path, "/", sizeof(full_path));
        ft_strlcat(full_path, argv[0], sizeof(full_path));
        if (access(full_path, F_OK) == 0)
        {
            argv[0] = arena_alloc(allocs->exec_alloc, ft_strlen(full_path) + 1);
            ft_strlcpy(argv[0], full_path, ft_strlen(full_path) + 1);
            return ;
        }
        i++;
    }
}

t_result    run_simple_cmd(t_ast *ast, t_mshell *shell, t_allocs *allocs)
{
    t_in_out    *io;
    pid_t       pid;

    if (!ast)
        return (create_error(ERROR));
    io = ast->data.cmd_node.io;
    pid = fork();
    if (pid < 0)
        return create_error(PID_ERROR);
    if (pid == 0)
    {
        if (io)
        {
            if (set_in_fds(io))
                return (create_error(ERROR));
            if (set_out_fds(io))
                return (create_error(ERROR));
        }
        add_full_path(ast->data.cmd_node.argv, shell->paths, allocs);
        execve(ast->data.cmd_node.argv[0], ast->data.cmd_node.argv, shell->env);
        perror("execve");
        return (create_error(ERROR));
    }
    if (waitpid(pid, NULL, 0) == 0)
        return (create_success(ast));
    return (create_error(ERROR));
}

void    remove_leading_quote(t_ast *ast)
{
    int i;

    i = 0;
    while (i < ast->data.cmd_node.argc)
    {
        if (ast->data.cmd_node.argv[i][0] == '"'
            || ast->data.cmd_node.argv[i][0] == '\'')
            ast->data.cmd_node.argv[i] = ast->data.cmd_node.argv[i] + 1;
        i++;
    }
}

static int has_input_redirection(t_in_out *io)
{
    return io && (io->in_mode == 0 || io->in_mode == 1);
}

static int has_output_redirection(t_in_out *io)
{
    return io && (io->out_mode == 0 || io->out_mode == 1);
}

int handle_pipeline(t_ast **cmds, int ncmds, t_mshell *shell, t_allocs *allocs, t_table *table)
{
    int pipefds[ncmds - 1][2];
    pid_t pids[ncmds];

    // Create all required pipes
    for (int i = 0; i < ncmds - 1; i++) {
        if (pipe(pipefds[i]) < 0) {
            perror("pipe");
            return 1;
        }
    }

    // Fork processes for each command
    for (int i = 0; i < ncmds; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            return 1;
        }

        if (pids[i] == 0) {
            // Child process

            // Apply input/output redirections
            t_in_out *io = cmds[i]->data.cmd_node.io;
            if (io) {
                if (io->in_mode == 0 || io->in_mode == 1) {
                    if (set_in_fds(io)) {
                        exit(EXIT_FAILURE);
                    }
                }
                if (io->out_mode == 0 || io->out_mode == 1) {
                    if (set_out_fds(io)) {
                        exit(EXIT_FAILURE);
                    }
                }
            }

            // Connect to previous pipe if no input redirection
            if (i > 0 && !has_input_redirection(io)) {
                dup2(pipefds[i - 1][0], STDIN_FILENO);
            }

            // Connect to next pipe if no output redirection
            if (i < ncmds - 1 && !has_output_redirection(io)) {
                dup2(pipefds[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends
            for (int j = 0; j < ncmds - 1; j++) {
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }

            // Perform substitutions and quote removal
            expand_substitutions(cmds[i], allocs, table);
            remove_leading_quote(cmds[i]);

            // Set full path and execute
            add_full_path(cmds[i]->data.cmd_node.argv, shell->paths, allocs);
            execve(cmds[i]->data.cmd_node.argv[0], cmds[i]->data.cmd_node.argv, shell->env);
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process closes all pipe ends
    for (int j = 0; j < ncmds - 1; j++) {
        close(pipefds[j][0]);
        close(pipefds[j][1]);
    }

    // Wait for all child processes
    for (int i = 0; i < ncmds; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}

void  walk_ast(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table)
{
    if (!ast) return;

    switch (ast->type)
    {
        case NODE_CMD:
        {
            expand_substitutions(ast, allocs, table);
            remove_leading_quote(ast);
            run_simple_cmd(ast, shell, allocs);
            break;
        }
        case NODE_PIPELINE:
        {
            t_ast   *cmds[128];
            int  n = collect_pipeline_cmds(ast, cmds, 128);
            if (n < 0) {
                fprintf(stderr, "Pipeline too deep\n");
                return;
            }
            handle_pipeline(cmds, n, shell, allocs, table);
            break;
        }
        case NODE_AND:
            printf("AND\n");
            walk_ast(ast->data.bin_op_node.left, shell, allocs, table);
            walk_ast(ast->data.bin_op_node.right, shell, allocs, table);
            break;

        case NODE_OR:
            printf("OR\n");
            walk_ast(ast->data.bin_op_node.left, shell, allocs, table);
            walk_ast(ast->data.bin_op_node.right, shell, allocs, table);
            break;

        case NODE_BACKGROUND:
            printf("BACKGROUND\n");
            /* background only has a left subtree */
            walk_ast(ast->data.sub, shell, allocs, table);
            break;

        case NODE_SUBSHELL:
            printf("SUBSHELL\n");
            walk_ast(ast->data.sub, shell, allocs, table);
            break;

        default:
            fprintf(stderr, "Unknown AST node type %d\n", ast->type);
            break;
    }
}

t_result    run_command(t_mshell *shell, t_allocs *allocs, t_table *table)
{
    t_result    result;

    result = get_env(table, "PATH");
    if (result.is_error)
        return (result);
    walk_ast(shell->ast, shell, allocs, table);
    arena_reset(allocs->exec_alloc);
    arena_reset(allocs->parse_alloc);
    return (create_success(shell->ast));
}
