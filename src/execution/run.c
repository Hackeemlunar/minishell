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
#include <fcntl.h>
#include <unistd.h>

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

int collect_heredoc_input(const char *delim, char *temp_file)
{
    int fd;
    char *line;

    fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("error in heredoc");
        return (-1);
    }
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
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    return (0);
}

void    get_inputs(t_in_out *io)
{
    char    *input;
    int     fd;

    if (!io->heredoc_delim)
        return ;
    fd = open("/tmp/heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return ;
    while (1)
    {
        input = readline("heredoc> ");
        if (!input || ft_strcmp(input, io->heredoc_delim) == 0)
            break ;
        if (ft_strlen(input) == 0)
        {
            free(input);
            continue ;
        }
        write(fd, input, ft_strlen(input));
        write(fd, "\n", 1);
        free(input);
    }
    free(input);
}

t_result    run_simple_cmd(t_ast *ast, t_mshell *shell, t_allocs *allocs)
{
    t_in_out    *io;
    pid_t       pid;

    (void)allocs;
    if (!ast)
        return (create_error(ERROR));
    io = ast->data.cmd_node.io;
    if (io->in_mode == 1 && io->heredoc_delim)
    {
        if (collect_heredoc_input(io->heredoc_delim, "/tmp/heredoc") < 0)
            return (create_error(ERROR));
        io->in_fd = open("/tmp/heredoc", O_RDONLY);
        if (io->in_fd < 0)
            return (create_error(NO_FILE));
        // dup2(io->in_fd, STDIN_FILENO);
    }
    pid = fork();
    if (pid < 0)
        return create_error(PID_ERROR);
    if (pid == 0)
    {
        if (io)
        {
            if (io->in_mode == 0 && io->in_file)
            {
                io->in_fd = open(io->in_file, O_RDONLY);
                if (io->in_fd < 0)
                    return (create_error(NO_FILE));
                dup2(io->in_fd, STDIN_FILENO);
            }
            if (io->in_fd != STDIN_FILENO)
                io->in_fd = STDIN_FILENO;
            if (io->out_mode == 0 && io->out_file)
            {
                io->out_fd = open(io->out_file, O_RDONLY);
                if (io->out_fd < 0)
                    return (create_error(NO_FILE));
                dup2(io->out_fd, STDOUT_FILENO);
            }
            else if (io->out_mode == 1 && io->out_file)
            {
                io->out_fd = open(io->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (io->out_fd < 0)
                    return (create_error(NO_FILE));
                dup2(io->out_fd, STDOUT_FILENO);
            }
        }
        execve(ast->data.cmd_node.argv[0], ast->data.cmd_node.argv, shell->env);
        perror("execve");
        return (create_error(ERROR));
    }
    if (waitpid(pid, NULL, 0) == 0)
        return (create_success(ast));
    unlink("/tmp/heredoc");
    return (create_error(ERROR));
}

char    *expand_variable(char *str, t_allocs *allocs, t_table *table)
{
    char        *expanded;
    char        *temp;
    t_result    value;
    int         i;

    while (str[0] == '"' && *str)
    {
        if (*str == '$')
        {
            str++;
            i = 0;
            while (str[i] && !is_special_char(str[i]))
                i++;
            temp = ft_substr(str, 0, i);
            value = get_env(table, temp);
            free(temp);
            if (value.is_error)
                return (printf("Unknown var: %s\n", str), NULL);
            expanded = arena_alloc(allocs->exec_alloc, ft_strlen(value.data.value) + 1);
            ft_strlcpy(expanded, value.data.value, ft_strlen(value.data.value) + 1);
            return (expanded);
        }
        str++;
    }
    return (NULL);
}

void expand_substitutions(t_ast *ast, t_allocs *allocs, t_table *table)
{
    int     i;
    char    *expanded;

    if (ast->type == NODE_CMD)
    {
        i = 0;
        while (ast->data.cmd_node.argv[i])
        {
            expanded = expand_variable(ast->data.cmd_node.argv[i], allocs, table);
            if (expanded)
            {
                printf("Expanded: %s\n", expanded);
                ast->data.cmd_node.argv[i] = expanded;
            }
            i++;
        }
    }
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

            for (int i = 0; i < n; i++)
            {
                if (i > 0) printf("PIPE\n");
                walk_ast(cmds[i], shell, allocs, table);
            }
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
