/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 17:20:16 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/11 09:59:00 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	here_doc_engine(char **sequence, char **hd, char *delimiter)
{
	free(*hd);
	*hd = "";
	*hd = readline("> ");
	if (ft_strcmp(*hd, delimiter) == 0)
		return ;
	if (*hd == NULL)
	{
		ft_fprintf(1, "bash: warning: here-document ");
		ft_fprintf(1, "delimited by end-of-file (wanted `%s')\n", delimiter);
	}
	if (g_error == 128 + SIGINT)
	{
		free(*sequence);
		free(*hd);
		exit(128 + SIGINT);
	}
	*sequence = path_join(*sequence, *hd);
	*sequence = path_join(*sequence, "\n");
}

static void	wait_for_hd(int pipe_fd[2], int fork_id, int *wstatus)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	waitpid(fork_id, wstatus, 0);
}

void	here_doc(char *delimiter)
{
	char	*hd;
	char	*sequence;
	int		pipe_fd[2];
	int		fork_id;

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"));
	set_signal_action(ON_FD);
	fork_id = fork();
	if (fork_id < 0)
		return (perror("fork"));
	if (fork_id == 0)
	{
		g_error = 0;
		sequence = ft_strdup("");
		hd = ft_strdup("");
		while (hd && ft_strcmp(hd, delimiter) != 0)
			here_doc_engine(&sequence, &hd, delimiter);
		free(hd);
		ft_fprintf(pipe_fd[1], "%s", sequence);
		free(sequence);
		exit(EXIT_SUCCESS);
	}
	wait_for_hd(pipe_fd, fork_id, NULL);
	set_signal_action(ON_BACKGROUND);
}
