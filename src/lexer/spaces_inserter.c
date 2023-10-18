/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces_inserter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 22:18:32 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/13 17:09:14 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_last_token_pos(char *str, int pos, int size)
{
	if (pos + size == (int)ft_strlen(str))
		return (1);
	return (0);
}

int	get_spaces_to_insert(char *str, int pos, int size)
{
	int	spaces_to_insert;

	spaces_to_insert = 2;
	if (pos == 0)
		spaces_to_insert --;
	else
		spaces_to_insert -= str[pos - 1] == ' ';
	if (is_last_token_pos(str, pos, size))
		spaces_to_insert --;
	else
		spaces_to_insert -= str[pos + size] == ' ';
	return (spaces_to_insert);
}

/**
 * @brief surrounds with spaces a substring./
 * @param str the whole string
 * @param pos where the substring to be surrounded starts
 * @param gap the length of the substring to be surrounded
 * @return  the number of spaces inserted.
*/
int	insert_spaces(char **str, int pos, int gap)
{
	char	*temp;
	int		spaces;

	spaces = get_spaces_to_insert(*str, pos, gap);
	temp = *str;
	*str = ft_calloc(ft_strlen(*str) + spaces + 1, sizeof(char));
	if (pos == 0)
		ft_strlcpy(*str, temp, gap + 1);
	else
	{
		ft_strlcpy(*str, temp, pos + 1);
		if (temp[pos - 1] != ' ')
			(*str)[pos] = ' ';
		ft_strlcpy(*str + pos + (temp[pos - 1] != ' '), temp + pos, gap + 1);
	}
	if (!is_last_token_pos(temp, pos, gap))
	{
		if (temp[pos + gap] != ' ')
			(*str)[pos + gap + spaces - 1] = ' ';
		ft_strlcpy(*str + ft_strlen(*str), temp + pos + gap, \
			ft_strlen(temp) - pos - gap + 1);
	}
	free(temp);
	return (spaces);
}
