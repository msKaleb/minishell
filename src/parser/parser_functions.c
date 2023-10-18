/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 20:32:13 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/17 13:30:15 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_input(char *file, int mode, int std_io)
{
	int	fd;

	fd = open(file, mode, 0644);
	if (fd == -1)
	{
		write(2, MSH_ERR, ft_strlen(MSH_ERR));
		write(2, file, ft_strlen(file));
		write(2, ": ", 2);
		perror("");
		g_error = B_GENERIC;
		return (-1);
	}
	dup2(fd, std_io);
	close(fd);
	return (EXIT_SUCCESS);
}

/**
* @brief Flushes the input and output
*/
int	restore_io(int fdin, int fdout)
{
	if (dup2(fdin, STDIN_FILENO) == -1)
		perror("dup2");
	if (dup2(fdout, STDOUT_FILENO) == -1)
		perror("dup2");
	if (close(fdin) == -1)
		perror("close");
	if (close(fdout) == -1)
		perror("close");
	return (EXIT_SUCCESS);
}

int	is_redirector(char *str)
{
	char	**reds;
	int		i;

	if (!str)
		return (0);
	reds = ft_split("< > << >>", ' ');
	i = -1;
	while (reds[++i])
	{
		if (ft_strcmp(str, reds[i]) == 0)
			return (dbl_free(reds), 1);
	}
	return (dbl_free(reds), 0);
}

/**
 * @brief Checks if the sequence has to be executed,
 * e.g a nonexistent input file
*/
int	check_inputs(t_msh *tm)
{
	char	*ofile;
	int		i;
	int		res;

	i = -1;
	ofile = NULL;
	while (tm->l_exp[++i])
	{
		res = op_selector(tm, i, &ofile, 1);
		if (res == -1)
			return (input_error(tm, &ofile));
		if (ft_strcmp(tm->l_exp[i], "cd") == 0 && i > 0 \
			&& is_pipe(tm, i - 1, tm->l_exp[i - 1]))
			return (-1);
		i += res;
		if (!tm->l_exp[i])
			break ;
		if (ofile)
			free(ofile);
		ofile = NULL;
	}
	free(tm->ofile);
	tm->ofile = NULL;
	return (0);
}

int	create_output(t_msh *tm, char *file, int mode, char **ofile)
{
	int	fd;

	if (file == NULL)
	{
		g_error = B_GENERIC;
		ft_fprintf(2, MSH_ERR"ambiguous redirect\n");
		return (-1);
	}
	if (*ofile)
	{
		free(*ofile);
		*ofile = NULL;
	}
	*ofile = ft_strdup(file);
	tm->out_mode = mode;
	fd = open(*ofile, mode, 0644);
	if (fd == -1)
	{
		ft_perror(B_GENERIC, ofile, strerror(errno), 0);
		return (-1);
	}
	close(fd);
	return (1);
}
