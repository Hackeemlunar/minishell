

#include "executor.h"

static void	run_child_cmd(t_ast *ast, t_mshell *shell, t_allocs *allocs)
{
	t_in_out	*io;

	io = ast->data.cmd_node.io;
	if (io)
	{
		if (set_in_fds(io))
			exit(1);
		if (set_out_fds(io))
			exit(1);
	}
	add_full_path(ast->data.cmd_node.argv, shell->paths, allocs);
	execve(ast->data.cmd_node.argv[0], ast->data.cmd_node.argv, shell->env);
	if (errno == ENOENT)
	{
		ft_putstr_fd(ast->data.cmd_node.argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	perror("execve");
	exit(126);
}

t_result	run_simple_cmd(t_ast *ast, t_mshell *shell, t_allocs *allocs)
{
	pid_t	pid;
	int		status;

	if (!ast)
		return (create_error(ERROR));
	shell->exit_status = 0;
	pid = fork();
	if (pid < 0)
		return (create_error(PID_ERROR));
	if (pid == 0)
		run_child_cmd(ast, shell, allocs);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_exit_status(shell, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(shell, 128 + WTERMSIG(status));
	else
		set_exit_status(shell, 1);
	return (create_success(ast));
}
