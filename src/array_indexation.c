/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_indexation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 12:42:41 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/05 12:43:22 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_quotes(char *token)
{
	int		i;
	char	*qsi;

	qsi = NULL;
	get_quote_state_i(token, &qsi);
	if (!qsi)
		return (-1);
	i = ft_strlen(qsi);
	while (i > 0 && qsi[--i])
	{
		if (is_quote(token[i]) && \
			(qsi[i] == token[i] || qsi[i] == '_' || \
			i == 0))
		{
			free(qsi);
			return (1);
		}
	}
	free(qsi);
	return (0);
}

void	is_quoted_array(t_msh *tm)
{
	int	i;

	i = 0;
	while (tm->l_exp[i])
		i++;
	tm->is_quoted = ft_calloc(i, sizeof(int));
	i = -1;
	while (tm->l_exp[++i])
		if (has_quotes(tm->l_exp[i]))
			tm->is_quoted[i] = 1;
}
