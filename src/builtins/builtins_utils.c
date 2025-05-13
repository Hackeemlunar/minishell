#include "builtins.h"


int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));         
}


int exec_builtin(char **argv, t_mshell *shell, t_table *table, int *exit_status)
{
    if (!argv || !argv[0])
        return (0);

    if (!ft_strcmp(argv[0], "echo"))
        echo(argv);
    else if (!ft_strcmp(argv[0], "pwd"))
        pwd(exit_status);
    else if (!ft_strcmp(argv[0], "env"))
        env(argv, table);
    else if (!ft_strcmp(argv[0], "export"))
        export_command(argv, table, exit_status);
    else if (!ft_strcmp(argv[0], "cd"))
        cd(argv, table); 
    else if (!ft_strcmp(argv[0], "unset"))
        unset(argv, &table->bucket[0]); 
    else if (!ft_strcmp(argv[0], "exit"))
        ft_exit(argv, shell); 
    else
        return (0);

    return (1);
}
