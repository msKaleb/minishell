/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 19:14:52 by aperez-m          #+#    #+#             */
/*   Updated: 2023/08/28 12:42:55 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief checks wether a position in a string is quoted
*/
void	get_quote_state(char *quote_state \
						, const char *char_checked \
						, const char *string_start)
{
	if (char_checked == string_start || *(char_checked - 1) != '\\')
	{
		if (*quote_state == '_')
		{
			if (is_quote(*char_checked))
				*quote_state = *char_checked;
		}
		else
			if (*char_checked == *quote_state)
				*quote_state = '_';
	}
}

int	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

/**
 * @brief replaces the open quote with a space
*/
void	fix_open_quotes(char **line_read)
{
	char	*quote_state_i;
	char	open_quote;
	int		i;

	quote_state_i = NULL;
	get_quote_state_i(*line_read, &quote_state_i);
	i = ft_strlen(quote_state_i);
	open_quote = quote_state_i[i - 1];
	while (--i >= 0)
	{
		if (quote_state_i[i] != open_quote)
			break ;
	}
	free(quote_state_i);
	if (i == 0)
		(*line_read)[0] = ' ';
	else
		(*line_read)[++i] = ' ';
}
