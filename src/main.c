/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:33:25 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/05 17:33:40 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_banner(void)
{
	printf("\n"
		"|************************** MiniShell v1.0 ***********************|\n"
		"|*                                                               *|\n"
		"|*                   Welcome to the MiniShell!                   *|\n"
		"|*                                                               *|\n"
		"|*                Created by \033[31m%s and %s\033[0m              "
		" *|\n"
		"|*                                                               *|\n"
		"|*               Type 'help' for a list of commands.             *|\n"
		"|*                                                               *|\n"
		"|*                     Type 'exit' to quit.                      *|\n"
		"|*                                                               *|\n"
		"|*                                                               *|\n"
		"|*                           Born2Code                           *|\n"
		"|*****************************************************************|\n"
		"\n", "Sngantch", "Hmensah-");
}

void	init_allocators(t_allocs *allocs)
{
	allocs->parse_alloc = arena_create(4096);
}
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
    /* Not a pipeline: it must be a command (or a subshell producing a single "cmd"). */
    out_cmds[0] = node;
    return 1;
}

void  walk_ast(t_ast *ast)
{
    if (!ast) return;

    switch (ast->type)
    {
        case NODE_CMD:
        {
            /* Print the command and all its args */
            printf("CMD:");
            for (int i = 0; i < ast->data.cmd_node.argc; i++)
                printf(" %s", ast->data.cmd_node.argv[i]);
            printf("\n");
            break;
        }
        case NODE_PIPELINE:
        {
            /* Flatten the pipeline into a list of cmds */
            t_ast *cmds[128];  /* adjust size or dynamically allocate */
            int  n = collect_pipeline_cmds(ast, cmds, 128);
            if (n < 0) {
                fprintf(stderr, "Pipeline too deep\n");
                return;
            }

            for (int i = 0; i < n; i++)
            {
                if (i > 0) printf("PIPE\n");
                walk_ast(cmds[i]);
            }
            break;
        }
        case NODE_AND:
            printf("AND\n");
            walk_ast(ast->data.bin_op_node.left);
            walk_ast(ast->data.bin_op_node.right);
            break;

        case NODE_OR:
            printf("OR\n");
            walk_ast(ast->data.bin_op_node.left);
            walk_ast(ast->data.bin_op_node.right);
            break;

        case NODE_BACKGROUND:
            printf("BACKGROUND\n");
            /* background only has a left subtree */
            walk_ast(ast->data.sub);
            break;

        case NODE_SUBSHELL:
            printf("SUBSHELL\n");
            walk_ast(ast->data.sub);
            break;

        default:
            fprintf(stderr, "Unknown AST node type %d\n", ast->type);
            break;
    }
}

/**
void walk_ast(t_ast *ast)
{
	t_ast *cur = ast;
	if (cur->type == NODE_CMD){
		printf("CMD: %s: %s: %d\n", cur->data.cmd_node.argv[0], cur->data.cmd_node.argv[1], cur->data.cmd_node.argc);
	}
	if (cur->type == NODE_PIPELINE){
		printf("PIPE LEFT\n");
		walk_ast(cur->data.bin_op_node.left);
		printf("PIPE RIGHT\n");
		walk_ast(cur->data.bin_op_node.right);
	}
	if (cur->type == NODE_AND){
		printf("AND LEFT\n");
		walk_ast(cur->data.bin_op_node.left);
		printf("AND RIGHT\n");
		walk_ast(cur->data.bin_op_node.right);
	}
	if (cur->type == NODE_OR){
		printf("OR LEFT\n");
		walk_ast(cur->data.bin_op_node.left);
		printf("OR RIGHT\n");
		walk_ast(cur->data.bin_op_node.right);
	}
	if (cur->type == NODE_BACKGROUND){
		printf("BACKGROUND\n");
		walk_ast(cur->data.bin_op_node.left);
	}
	if (cur->type == NODE_SUBSHELL){
		printf("SUBSHELL\n");
		walk_ast(cur->data.sub);
	}
}
*/

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	t_result	result;
	t_allocs	allocs;
	t_mshell	mshell;
	t_table		env_table;

	(void)argc;
	(void)argv;
	show_banner();
	init_allocators(&allocs);
	read_history("./histfile");
	init_env(&env_table, envp);
	printf("env_table size: %d\n", env_table.size);
	t_result res = get_env(&env_table, "USER");
	printf("env_table: %s\n", (char *)res.data.value);
	while (true)
	{
		str = readline("minishell> ");
		add_history(str);
		if (str == NULL)
		{
			printf("Error: readline failed\n");
			return (1);
		}
		result = parse_cmdln(str, &mshell, &allocs);
		if (result.is_error) {
			printf("%d\n", result.data.error);
			break ;
		}
		mshell.ast = result.data.value;
		walk_ast(mshell.ast);
		arena_reset(allocs.parse_alloc);
	}
	write_history("./histfile");
	clean_env(&env_table);
	arena_destroy(allocs.parse_alloc);
	free(str);
	return (0);
}
