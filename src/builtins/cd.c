#include "builtins.h"
// #include "../minishell.h"

static void update_pwd(t_table *table)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        write(STDERR_FILENO,
            "minishell: cd: error retrieving current directory\n", 50);
        return;
    }
    char *pwd = getcwd(NULL, 0);
    if (!pwd)
    {
        write(STDERR_FILENO,
            "minishell: cd: error retrieving current directory\n", 50);
        return;
    }

    t_result oldpwd_res = get_env(table, "PWD");
    if (!oldpwd_res.is_error && oldpwd_res.data.value)
    {
        t_env *oldpwd = (t_env *)oldpwd_res.data.value;
        if (oldpwd && oldpwd->value)
            add_env(table, "OLDPWD", oldpwd->value);
    }
    
    add_env(table, "PWD", pwd);
    free(pwd);
}

static void cd_home(t_table *table, int *exit_status)
{
    if (!table || !exit_status) return;

    t_result home_res = get_env(table, "HOME");
    if (home_res.is_error || !home_res.data.value)
    {
        write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
        *exit_status = 1;
        return;
    }

    t_env *home_entry = (t_env *)home_res.data.value;
    if (!home_entry || !home_entry->value)
    {
        write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
        *exit_status = 1;
        return;
    }
    
    if (chdir(home_entry->value) != 0)
    {
        perror("minishell: cd");
        *exit_status = 1;
        return;
    }
    update_pwd(table);
}

static void cd_dash(t_table *table, int *exit_status)
{
    if (!table || !exit_status) return;

    t_result oldpwd_res = get_env(table, "OLDPWD");
    if (oldpwd_res.is_error || !oldpwd_res.data.value)
    {
        write(STDERR_FILENO, "minishell: cd: OLDPWD not set\n", 30);
        *exit_status = 1;
        return;
    }

    t_env *oldpwd_entry = (t_env *)oldpwd_res.data.value;
    if (!oldpwd_entry || !oldpwd_entry->value)
    {
        write(STDERR_FILENO, "minishell: cd: OLDPWD not set\n", 30);
        *exit_status = 1;
        return;
    }
    
    if (chdir(oldpwd_entry->value) != 0)
    {
        perror("minishell: cd");
        *exit_status = 1;
        return;
    }
    ft_printf("%s\n", oldpwd_entry->value);
    update_pwd(table);
}

void cd(char **argv, t_table *table, int *exit_status)
{
    if (!table || !exit_status)
    {
        if (exit_status) *exit_status = 1;
        return;
    }

    if (!argv[1] || !argv[1])
    {
        cd_home(table, exit_status);
        return;
    }

    if (ft_strcmp(argv[1], "-") == 0)
    {
        cd_dash(table, exit_status);
        return;
    }

    if (chdir(argv[1]) != 0)
    {
        ft_printf("minishell: cd: %s: %s\n", argv[1], strerror(errno));
        *exit_status = 1;
        return;
    }
    update_pwd(table);
}
