/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:20:57 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/12 23:12:32 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief space separates a string into tokens
*/
void	lexer(char **line_read)
{
	int		i;
	char	*quote_state_i;
	char	*bsi;
	int		gap;

	i = -1;
	quote_state_i = NULL;
	while ((*line_read)[++i])
	{
		get_quote_state_i(*line_read, &quote_state_i);
		bsi = get_brace_state_i(*line_read);
		while (quote_state_i[i] && (quote_state_i[i] != '_' || bsi[i] != '_' ))
			i++;
		free (bsi);
		gap = is_special(line_read, i);
		if (gap == -1)
		{
			ft_perror(B_GENERIC, line_read, "Operator not managed", 2);
			break ;
		}
		if (!gap)
			continue ;
		i += insert_spaces(line_read, i, gap) + gap - 1;
	}
	free(quote_state_i);
}

/**
 * @return length of the special token,
 * 0 if it isn't special,
 * -1 if the special operator is forbidden
*/
int	is_special(char **str, int i)
{
	int	ret;
	int	ret2;

	ret = is_metacharacter((*str) + i);
	ret2 = 0;
	if (ret)
		ret2 = is_operator((*str) + i);
	if (ret2)
	{
		if (ret2 == -1)
		{
			free(*str);
			*str = NULL;
		}
		return (ret2);
	}
	return (ret);
}

int	is_metacharacter(char *c)
{
	if (ft_strchr("|&;()<>",*c))
		return (1);
	return (0);
}

int	is_operator(char *c)
{
	char	**opers;
	char	**forbidden_opers;
	int		ret;
	int		i;

	opers = ft_split("<< >>", ' ');
	forbidden_opers = ft_split \
		("|| |& &>> &> && ;;& ;; ;& <<< <> <& >| >&", ' ');
	i = -1;
	while (opers[++i])
	{
		ret = ft_strlen(opers[i]);
		if (!ft_strncmp(c, opers[i], ret))
			return (dbl_free(opers), dbl_free(forbidden_opers), ret);
	}
	i = -1;
	while (forbidden_opers[++i])
		if (!ft_strncmp(c, forbidden_opers[i], ft_strlen(forbidden_opers[i])))
			return (dbl_free(opers), dbl_free(forbidden_opers), -1);
	return (dbl_free(opers), dbl_free(forbidden_opers), 0);
}
