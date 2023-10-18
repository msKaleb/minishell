/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:57:31 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/14 11:23:43 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * @brief does whatever with the result of readline()
 * and adds it to the history
*/
char	*ft_readline(t_msh *tm)
{
	char	*prompt;
	char	*line_read;
	char	*temp;

	set_signal_action(ON_FT_READLINE);
	prompt = create_prompt(tm);
	line_read = readline(prompt);
	line_read = get_line_with_content(&line_read, prompt, tm);
	while (!(1 && check_quote_completion(line_read) \
		&& check_braces_completion(line_read)))
	{
		temp = line_read;
		line_read = ft_strjoin(line_read, "\n");
		free (temp);
		temp = line_read;
		line_read = ft_strjoin(line_read, readline(">"));
		free (temp);
	}
	return (add_history(line_read), free(prompt), line_read);
}

/**
 * @brief if line_read is empty it keeps trhowing a new prompt
*/
char	*get_line_with_content(char **line_read, char *prompt, t_msh *tm)
{
	while (!line_has_content(*line_read, tm))
	{
		free(*line_read);
		*line_read = readline(prompt);
	}
	return (*line_read);
}

int	line_has_content(char *line_read, t_msh *tm)
{
	exit_on_ctrl_d(line_read, tm);
	if (!line_read[0])
		return (0);
	return (ft_strlen(line_read));
}

void	exit_on_ctrl_d(char *line_read, t_msh *tm)
{
	if (line_read == NULL)
	{
		write(1, "exit\n", 5);
		ft_lstclear(tm->tenvp, &free);
		free(tm->ofile);
		exit (0);
	}
}
