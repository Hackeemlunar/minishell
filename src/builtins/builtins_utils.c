#include "builtins.h"

int	exec_builtin(char **argv, t_mshell *shell)
{
	if (!ft_strcmp(argv[0], "echo"))
		echo(argv);
	else if (!ft_strcmp(argv[0], "pwd"))
		pwd();
	else if (!ft_strcmp(argv[0], "env"))
		env(argv, shell->params);
	else if (!ft_strcmp(argv[0], "export"))
		export_command(argv, &shell->params->env_var_list, &shell->exit_status);
	//  cd, unset, exit later 
	else
		return (0);
	return (1);
}

if (is_builtin(ast->data.cmd_node.argv[0]) && !in_pipeline)
{
	// run directly in parent
	exec_builtin(ast->data.cmd_node.argv, shell);
	return (create_success(ast));
}
else
{
	// fork, then call exec_builtin inside child (if builtin), else execve
}
