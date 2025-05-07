/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:52:11 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/07 19:12:57 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <sys/_types/_pid_t.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
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

void    get_inputs(void)
{

}

t_result    run_simple_cmd(t_ast *ast, char **env)
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
            if (io->in_mode == 0 && io->in_file)
            {
                io->in_fd = open(io->in_file, O_RDONLY);
                if (io->in_fd < 0)
                    return (create_error(NO_FILE));
                dup2(io->in_fd, STDIN_FILENO);
            }
            else if (io->in_mode == 1 && io->heredoc_delim)
            {
                get_inputs();
                io->in_fd = open("/tmp/heredoc", O_RDONLY);
                if (io->in_fd < 0)
                    return (create_error(NO_FILE));
                dup2(io->in_fd, STDIN_FILENO);
            }
            else
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
            else
                io->in_fd = STDOUT_FILENO;
        }
        execve(ast->data.cmd_node.argv[0], ast->data.cmd_node.argv, env);
        perror("execve");
        return (create_error(ERROR));
    }
    if (waitpid(pid, NULL, 0) == 0)
        return (create_success(ast));
    return (create_error(ERROR));
}

void  walk_ast(t_ast *ast, char **env)
{
    if (!ast) return;

    switch (ast->type)
    {
        case NODE_CMD:
        {
           run_simple_cmd(ast, env);
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
                walk_ast(cmds[i], env);
            }
            break;
        }
        case NODE_AND:
            printf("AND\n");
            walk_ast(ast->data.bin_op_node.left, env);
            walk_ast(ast->data.bin_op_node.right, env);
            break;

        case NODE_OR:
            printf("OR\n");
            walk_ast(ast->data.bin_op_node.left, env);
            walk_ast(ast->data.bin_op_node.right, env);
            break;

        case NODE_BACKGROUND:
            printf("BACKGROUND\n");
            /* background only has a left subtree */
            walk_ast(ast->data.sub, env);
            break;

        case NODE_SUBSHELL:
            printf("SUBSHELL\n");
            walk_ast(ast->data.sub, env);
            break;

        default:
            fprintf(stderr, "Unknown AST node type %d\n", ast->type);
            break;
    }
}

t_result    run_command(t_mshell *shell, t_allocs *allocs, t_table *table)
{
    (void)allocs;
    (void)table;
    walk_ast(shell->ast, shell->env);
    return (create_success(shell->ast));
}
