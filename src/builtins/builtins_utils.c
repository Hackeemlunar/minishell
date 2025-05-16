#include "builtins.h"

int handle_builtins(char **argv, t_mshell *sh, t_table *table, t_allocs *alloc)
{
    if (!argv || !argv[0] || !sh || !table)
        return (1);
    if (ft_strcmp(argv[0], "echo") == 0)
        echo(argv);
    else if (ft_strcmp(argv[0], "pwd") == 0)
        pwd(&sh->exit_status);
    else if (ft_strcmp(argv[0], "env") == 0)
        env(argv, table);
    else if (ft_strcmp(argv[0], "export") == 0)
        export_command(argv, table, &sh->exit_status);
    else if (ft_strcmp(argv[0], "cd") == 0)
        cd(argv, table, &sh->exit_status); 
    else if (ft_strcmp(argv[0], "unset") == 0)
        unset(argv, table, &sh->exit_status); 
    else if (ft_strcmp(argv[0], "exit") == 0)
        ft_exit(argv, sh, alloc); 
    else
        return (1);
    return (0);
}
