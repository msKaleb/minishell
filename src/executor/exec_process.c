/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:19:19 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/17 17:01:57 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the command has to be implemented for the project
 * @param cmd Double pointer for accessing the arguments passed via cmd[1], etc
*/
static int	exec_built_ins(char **cmd, t_msh *tm, char ***envp_addr, int child)
{
	int	ret;

	if (ft_strcmp(cmd[0], "env") == 0)
		return (env(tm->tenvp));
	if (ft_strcmp(cmd[0], "pwd") == 0)
		return (pwd());
	if (ft_strcmp(cmd[0], "cd") == 0)
		return (cd(cmd, *tm->tenvp, envp_addr));
	if (ft_strcmp(cmd[0], "echo") == 0)
		return (echo(cmd));
	if (ft_strcmp(cmd[0], "export") == 0)
		return (export(cmd, *tm->tenvp, envp_addr, 0));
	if (ft_strcmp(cmd[0], "unset") == 0)
	{
		ret = unset(cmd, tm->tenvp, envp_addr);
		return (ret);
	}
	if (ft_strcmp(cmd[0], "exit") == 0)
	{
		if (!child)
			if (!check_exit_arg(cmd[1], cmd))
				return (ft_exit(tm, 255));
		return (ft_exit(tm, ft_atoi(cmd[1])));
	}
	return (0);
}

/**
 * @brief Generates the absolute path of the command being executed.
 * It checks first if the command passed is in absolute path, if not,
 * the command is appended to its path for execve() function to work.
 * @param cmd The split double pointer with the command and its arguments
 * @param paths The split double pointer with system paths
 * 	from $PATH enviroment variable
 * @note returns "\024" instead of NULL to avoid valgrind error
*/
static char	*get_cmd_path(char **cmd, t_msh *tm, char ***envp_addr)
{
	char	*tmp_path;
	char	*path_cmd;
	int		i;

	if (exec_built_ins(cmd, tm, envp_addr, 1))
		return ("\024");
	if (access(cmd[0], F_OK | X_OK) == 0)
		return (cmd[0]);
	i = -1;
	if (!tm->paths)
		return ("\024");
	while (tm->paths && tm->paths[++i])
	{
		tmp_path = ft_strjoin(tm->paths[i], "/");
		path_cmd = ft_strjoin(tmp_path, cmd[0]);
		free(tmp_path);
		if (access(path_cmd, F_OK | X_OK) == 0)
			return (path_cmd);
		free(path_cmd);
	}
	path_cmd = ft_strdup("");
	if (access(cmd[0], F_OK) == 0 || access(path_cmd, F_OK) == 0)
		g_error = B_NOT_PERM;
	free(path_cmd);
	return ("\024");
}

void	dup_input(int pipe_fd[2], char **args)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	dbl_free(args);
}

/**
*pipe_fd[0] - read end of the pipe
*pipe_fd[1] - write end of the pipe
*fork_id  = 0: child process, writes on STDOUT
*fork_id != 0: parent process, gets the output of child and uses it as STDIN
*/
void	exec_child(char *command, t_msh *tm)
{
	t_process	proc;
	char		**args;

	args = ft_split_args(command);
	if (pipe(proc.pipe_fd) < 0)
		return (perror("pipe"));
	proc.fork_id = fork();
	if (proc.fork_id < 0)
		return (perror("fork"));
	add_fork_id(proc.fork_id, tm);
	if (proc.fork_id == 0)
	{
		dup2(proc.pipe_fd[1], STDOUT_FILENO);
		dup_ofile(&tm);
		close(proc.pipe_fd[0]);
		close(proc.pipe_fd[1]);
		execve(get_cmd_path(args, tm, &(tm->envp)), \
			args, tm->envp);
		if (!is_built_in(args[0]))
			cmd_not_found(tm, args);
		ft_exit(tm, EXIT_FAILURE);
	}
	else
		dup_input(proc.pipe_fd, args);
}

// g_error = 130; // 128 + WTERMSIG(proc.wstatus) ???
/**
 * @brief Executes the last command in sequence
 */
void	exec_parent(char *command, t_msh *tm)
{
	t_process	proc;
	char		**args;

	proc.wstatus = 0;
	args = ft_split_args(command);
	if (pipe_and_fork(&proc) == -1)
		return ;
	dup_ofile(&tm);
	if (proc.fork_id == 0)
	{
		if (is_built_in(args[0]))
			ft_exit(tm, EXIT_SUCCESS);
		execve(get_cmd_path(args, tm, &(tm->envp)), args, tm->envp);
		cmd_not_found(tm, args);
	}
	waitpid(proc.fork_id, &proc.wstatus, 0);
	dup_input(proc.pipe_fd, NULL);
	wait_for_children(tm);
	g_error = WEXITSTATUS(proc.wstatus);
	if (WIFSIGNALED(proc.wstatus))
		g_error = 130;
	exec_built_ins(args, tm, &(tm->envp), 0);
	dbl_free(args);
	export_last_command(command, tm);
}
