/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_msh.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 18:53:39 by aperez-m          #+#    #+#             */
/*   Updated: 2023/08/30 17:32:28 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
*/
int	get_w_nbr_msh(char const *s, char c, char *quote_state_i, char *bsi)
{
	int		i;
	int		w_nbr;

	i = 0;
	w_nbr = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] && (quote_state_i[i] != '_' || bsi[i] != '_' || s[i] != c))
			w_nbr++;
		while (s[i] && (quote_state_i[i] != '_' || bsi[i] != '_' || s[i] != c))
			i++;
	}
	return (w_nbr);
}

/**/
int	get_w_len_msh(char const *s, char c, char *quote_state_i, char *bsi)
{
	int		w_len;

	w_len = 0;
	while (s[w_len] && \
		(quote_state_i[w_len] != '_' || bsi[w_len] != '_' || s[w_len] != c))
		w_len++;
	return (w_len);
}

char	**err_free_msh(char **list, int i)
{
	while (i > 0)
	{
		i--;
		free(list[i]);
	}
	free(list);
	return (NULL);
}

/**
 * @param tgi token_groupers_index. tgi[0] = a string that holds
 * the quoting state for every char of s. tgi[1] = a string that
 * holds the bracing state for every char of s. 
*/
char	**build_list_msh(char **ret, char const *s, char c, char **tgi)
{
	int		w_len;
	int		i;

	i = -1;
	while (*s)
	{
		while (*s == c)
		{
			s++;
			tgi[0]++;
			tgi[1]++;
		}
		if (*s)
		{
			w_len = get_w_len_msh(s, c, tgi[0], tgi[1]);
			ret[++i] = (char *)malloc(w_len + 1);
			if (!ret[i])
				return (err_free_msh(ret, i));
			ft_strlcpy(ret[i], s, w_len + 1);
			s += w_len;
			tgi[0] += w_len;
			tgi[1] += w_len;
		}
	}
	return (ret);
}

char	**ft_split_msh(char const *s, char c)
{
	char	**ret;
	int		w_nbr;
	char	**token_groupers_i;
	char	**dummy;

	token_groupers_i = ft_calloc(3, sizeof (char *));
	dummy = ft_calloc(3, sizeof (char *));
	token_groupers_i[0] = NULL;
	get_quote_state_i(s, token_groupers_i);
	token_groupers_i[1] = get_brace_state_i(s);
	dummy[0] = token_groupers_i[0];
	dummy[1] = token_groupers_i[1];
	w_nbr = get_w_nbr_msh(s, c, token_groupers_i[0], token_groupers_i[1]);
	ret = (char **)malloc((w_nbr + 1) * sizeof(char *));
	if (!ret)
		return (dbl_free(token_groupers_i), free(dummy), NULL);
	if (!build_list_msh(ret, s, c, dummy))
		return (dbl_free(token_groupers_i), free(dummy), NULL);
	ret[w_nbr] = NULL;
	if (!token_groupers_i[1][0])
		free(token_groupers_i[1]);
	return (dbl_free(token_groupers_i), free(dummy), ret);
}
