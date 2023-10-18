/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 22:33:33 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/13 16:50:27 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_line_brace_complete(char **line_read, char *prompt, t_msh *tm)
{
	char	*temp;

	while (!check_braces_completion(*line_read))
	{
		temp = *line_read;
		*line_read = ft_strjoin(*line_read, readline(prompt));
		free (temp);
		*line_read = get_line_quote_complete(line_read, prompt, tm);
	}
	return (*line_read);
}

/**
 * @return 1 if braces complete. 0 Otherwise
*/
int	check_braces_completion(char *line_read)
{
	char	*braces_state_i;

	braces_state_i = get_brace_state_i(line_read);
	if (braces_state_i[ft_strlen(braces_state_i) - 1] != '_')
	{
		printf(ANSI_RED"Open braces not supported"ANSI_RESET"\n");
		free(braces_state_i);
		return (0);
	}
	free(braces_state_i);
	return (1);
}

static char	*init_braces_state_i(const char *str, int *i);

char	*get_brace_state_i(const char *str)
{
	int		aux[2];
	char	*ret;
	char	*qt_i;

	ret = init_braces_state_i(str, aux);
	if (!ret || ft_strlen(str) < 2)
		return (ret);
	qt_i = NULL;
	get_quote_state_i(str, &qt_i);
	while (str[++aux[0]])
	{
		ret[aux[0]] = ret[aux[0] - 1];
		aux[1] += (ret[aux[0]] == '{' && qt_i[aux[0] - 1] != '\'' \
			&& qt_i[aux[0]] != '\'' && !ft_strncmp(str + aux[0] - 1, "${", 2));
		if (ret[aux[0]] == '_' && qt_i[aux[0] - 1] != '\'' \
			&& qt_i[aux[0]] != '\'' && !ft_strncmp(str + aux[0] - 1, "${", 2))
			ret[aux[0]] = '{';
		if (!aux[1] && str[aux[0]] == '}' && ret[aux[0]] == '{' \
			&& qt_i[aux[0]] != '\'' )
			ret[aux[0]] = '_';
		aux[1] -= (aux[1] && str[aux[0]] == '}' && ret[aux[0]] == '{' \
			&& qt_i[aux[0]] != '\'');
	}
	free(qt_i);
	return (ret);
}

static char	*init_braces_state_i(const char *str, int *aux)
{
	char	*ret;

	if (!str)
		return (NULL);
	aux[0] = 1;
	aux[1] = 0;
	ret = ft_calloc(ft_strlen(str) + 1, 1);
	ft_memset(ret, '_', ft_strlen(str));
	if (!ft_strncmp(str, "${", 2))
		ret[1] = '{';
	return (ret);
}
