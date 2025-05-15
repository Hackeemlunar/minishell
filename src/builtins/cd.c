#include "builtins.h"
// #include "../minishell.h"

static void change_to_home_directory(int *exit_status, t_table *table)
{
    char* home_dir = get_env(table, "HOME").data.value;

    if (home_dir)
    {
        if (chdir(home_dir) != 0)
        {
            perror("cd");
            *exit_status = 1;
        }
    }
    else
    {
        ft_printf("minishell: cd: HOME not set\n");
        *exit_status = 1;
    }
}

static void update_pwd(t_table *table)
{
    char *pwd = getcwd(NULL, 0);
    t_env old_pwd_node;

    if (!pwd)
    {
        ft_printf("minishell: cd: error retrieving current directory");
        write(STDERR_FILENO, "\n", 1);
        return;
    }
    t_env *tmp = table->bucket[0];

    while(tmp)
    {
        if (ft_strcmp(tmp->key, "PWD") == 0)
        {
            old_pwd_node.key = ft_strdup("OLDPWD");
            old_pwd_node.value = tmp->value;
            add_env(table, old_pwd_node.key, old_pwd_node.value);
            tmp->value = ft_strdup(pwd);
            free(pwd);
            return;
        }
        tmp = tmp->next;
    }
}

static void cd_oldpwd_check(t_table *table, int *exit_status)
{
    if (chdir(get_env(table, "OLDPWD").data.value) != 0)
    {
        ft_printf("minishell: cd: OLDPWD not set\n");
        write(STDERR_FILENO, "\n", 1);
        *exit_status = 1;
    }
    else
    {
        ft_printf("%s\n", get_env(table, "OLDPWD").data.value);
        update_pwd(table);
    }
}


static void cd_path_check(char **av, int *exit_status, t_table *table)
{
    char *path = ft_strjoin(get_env(table, "HOME").data.value, av[1] + 1);
    if (chdir(path) != 0)
    {
        ft_printf("minishell: cd: %s: No such file or directory", path);
        write(STDERR_FILENO, "\n", 1);
        *exit_status = 1;
    }
    else
        update_pwd(table);
    free(path);
}


/**
 * @brief Changes the current directory to the one given as an argument.
 *
 * @param av the arguments given to the cd command
 * @param table the environment table
 * @param exit_status the exit status of the cd command
 *
 * If the given argument is NULL, the cd command changes the current
 * directory to the value of the HOME environment variable.
 * If the given argument is "-", the cd command changes the current
 * directory to the value of the OLDPWD environment variable.
 * If the given argument begins with a '~', the cd command changes the
 * current directory to the value of the HOME environment variable
 * followed by the argument given after the '~'.
 * Otherwise, the cd command changes the current directory to the given
 * argument.
 *
 * If the given argument is not a valid directory, the cd command prints
 * an error message and sets the exit status to 1.
 *
 * The cd command always updates the PWD and OLDPWD environment variables.
 */
void cd(char **av, t_table *table, int *exit_status)
{
    if (av[1] == NULL)
    {
        change_to_home_directory(exit_status, table);
        update_pwd(table);
        return;
    }
    if (ft_strcmp(av[1], "-") == 0)
        cd_oldpwd_check(table, exit_status);
    else if (av[1][0] == '~')
        cd_path_check(av, exit_status, table);
    else if (chdir(av[1]) != 0)
    {
        ft_printf("minishell: cd: %s: No such file or directory", av[1]);
        write(STDERR_FILENO, "\n", 1);
        *exit_status = 1;
    }
    else
        update_pwd(table);
}
