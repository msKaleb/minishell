/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 08:21:29 by aperez-m          #+#    #+#             */
/*   Updated: 2023/08/30 06:31:00 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief fills quote_state_i
 * @param quote_state_i is ** because command.quote_state_i is char*
 * and i have to realloc it in a function different from the one i declared
 * it in.
 * @note mallocs, you need to free manually after usage.
*/
void	get_quote_state_i(char const *line_read, char **quote_state_i)
{
	int	i;

	if (*quote_state_i)
		free(*quote_state_i);
	if (!line_read || !line_read[0])
	{
		*quote_state_i = NULL;
		return ;
	}
	*quote_state_i = ft_calloc(ft_strlen(line_read) + 1, 1);
	ft_memset(*quote_state_i, '_', ft_strlen(line_read));
	get_quote_state(*quote_state_i, line_read, line_read);
	i = 0;
	while (line_read[++i])
	{
		(*quote_state_i)[i] = (*quote_state_i)[i - 1];
		get_quote_state((*quote_state_i) + i, line_read + i, line_read);
	}
}

	//choose, if open quotes, fixes input:
	//	fix_open_quotes(line_read);
	//or rejects input:
		//free(*line_read);
		//*line_read = readline(prompt);
	//or asks to complete
char	*get_line_quote_complete(char **line_read, char *prompt, t_msh *tm)
{
	while (!check_quote_completion(*line_read))
	{
		*line_read = ft_strjoin(*line_read, readline(">"));
		*line_read = get_line_with_content(line_read, prompt, tm);
	}
	return (*line_read);
}

/**
 * @return 1 if quote complete. 0 Otherwise. 
*/
int	check_quote_completion(char *line_read)
{
	char	*quote_state_i;
	char	last_char;

	quote_state_i = NULL;
	get_quote_state_i(line_read, &quote_state_i);
	if (!quote_state_i)
		return (0);
	last_char = quote_state_i[ft_strlen(quote_state_i) - 1];
	free(quote_state_i);
	if (last_char != '_')
		return (0);
	return (1);
}

/**
 * @brief removes all active quotes from a token.
 * the portion of the string after an active quote is
 * copied one position <-. The NULL terminator
 * is moved <- as many positions as quotes have
 * been removed, so the space allocated remains the same,
 * but the size of the string changes.
*/
void	quote_remove(char *token)
{
	int		i;
	char	*qsi;
	int		j;

	qsi = NULL;
	get_quote_state_i(token, &qsi);
	if (!qsi)
		return ;
	i = ft_strlen(qsi);
	while (i > 0 && qsi[--i])
	{
		if (is_quote(token[i]) && \
			(qsi[i] == token[i] || qsi[i] == '_' || \
			i == 0))
		{
			j = i - 1;
			while (token[++j])
				token[j] = token[j + 1];
		}
	}
	free(qsi);
}
