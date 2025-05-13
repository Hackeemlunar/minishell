#include "builtins.h"
// #include "../minishell.h"

static void	update_pwd(t_table *table)
{
	char *pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		write(STDERR_FILENO,
			"minishell: cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 119);
		return;
	}

	t_result oldpwd_res = get_env(table, "PWD");
	if (!oldpwd_res.is_error)
	{
		t_env *oldpwd = oldpwd_res.data.value;
		add_env(table, "OLDPWD", oldpwd->value); // update OLDPWD
	}
	add_env(table, "PWD", pwd); // update PWD
	free(pwd);
}

static void	cd_home(t_table *table, int *exit_status)
{
	t_result home_res = get_env(table, "HOME");
	if (home_res.is_error)
	{
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
		*exit_status = 1;
		return;
	}

	char *home_dir = ((t_env *)home_res.data.value)->value;
	if (chdir(home_dir) != 0)
	{
		perror("cd");
		*exit_status = 1;
		return;
	}
	update_pwd(table);
}

static void	cd_dash(t_table *table, int *exit_status)
{
	t_result oldpwd_res = get_env(table, "OLDPWD");
	if (oldpwd_res.is_error)
	{
        write(STDERR_FILENO, "minishell: cd: OLDPWD not set\n", 30);
		*exit_status = 1;
		return;
	}

	char *oldpwd = ((t_env *)oldpwd_res.data.value)->value;
	if (chdir(oldpwd) != 0)
	{
		perror("cd");
		*exit_status = 1;
		return;
	}
	ft_printf("%s\n", oldpwd);
	update_pwd(table);
}

void	cd(char **argv, t_table *table, int *exit_status)
{
	if (!argv[1])
		return cd_home(table, exit_status);

	if (!ft_strcmp(argv[1], "-"))
		return cd_dash(table, exit_status);

	if (chdir(argv[1]) != 0)
	{
		ft_printf("minishell: cd: %s: No such file or directory", argv[1]);
        write(STDERR_FILENO, "\n", 1);
		*exit_status = 1;
		return;
	}
	update_pwd(table);
}
