/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 14:10:48 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/16 19:53:45 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief takes a line read and split, so, a char **, and
 * glob and parametre expands every element and fills a char **
 * with independent tokens 
*/
char	**line_expander(char ***line_split, t_list *lenv)
{
	int		i;
	char	**ret;
	int		nmemb;
	char	*temp;

	i = -1;
	while ((*line_split)[++i])
		;
	nmemb = i;
	ret = ft_calloc(nmemb + 1, sizeof(char *));
	i = -1;
	while ((*line_split)[++i])
	{
		ret[i] = sh_par_expander((*line_split)[i], lenv);
		if (ft_strchr(ret[i], '*'))
		{
			temp = ret[i];
			ret[i] = fn_expander(ret[i]);
			free(temp);
		}
	}
	ret = unpack_char_pp(&ret);
	return (ret);
}

/**
 * @brief splits every char * of a char ** and fills a char **
 * with the resulting char *
 * @param char_pp adress of the char ** whose members will be split. 
*/
char	**unpack_char_pp(char ***char_pp)
{
	char	**ret;
	int		nmemb;
	int		i;
	char	*quote_state_i;
	char	*bsi;

	i = -1;
	nmemb = 0;
	quote_state_i = NULL;
	while ((*char_pp)[++i])
	{
		lexer(&(*char_pp)[i]);
		get_quote_state_i((*char_pp)[i], &quote_state_i);
		bsi = get_brace_state_i((*char_pp)[i]);
		nmemb += get_w_nbr_msh((*char_pp)[i], ' ', quote_state_i, bsi);
		free(bsi);
	}
	ret = ft_calloc(nmemb + 1, sizeof(char *));
	fill_char_pp(&ret, char_pp);
	free(quote_state_i);
	dbl_free(*char_pp);
	return (ret);
}

/**
 * @brief fills ret with src
 * @param ret_addr address of the char ** to be filled with all the
 * char * resulting from splitting every char * in src
 * @note doesnt malloc 
*/
void	fill_char_pp(char ***ret_addr, char ***src_addr)
{
	int		i;
	int		j;
	int		k;
	char	**temp;

	i = -1;
	j = -1;
	k = -1;
	while ((*src_addr)[++i])
	{
		temp = ft_split_msh((*src_addr)[i], ' ');
		j = -1;
		while (temp[++j])
			(*ret_addr)[++k] = ft_strdup(temp[j]);
		dbl_free(temp);
	}
}

/**
 * @brief substitutes part of the string host with the string insert
 * @param host the string that receives the insertion
 * @param insert the string inserted
 * @param d_pos where in host should 'insert' start, where the $ is
*/
char	*replace_string_chunk(char **host, char **insert, size_t d_pos)
{
	char	*ret;
	int		ret_size;
	char	*second_part;

	second_part = get_parameter_end(*host, d_pos) + 1;
	ret_size = d_pos + ft_strlen(*insert) + \
		ft_strlen(second_part) + 1;
	ret = ft_calloc(ret_size, 1);
	ft_strlcpy(ret, *host, d_pos + 1);
	if (insert && *insert)
		ft_strlcpy(ret + d_pos, *insert, ft_strlen(*insert) + 1);
	ft_strlcpy(ret + d_pos + ft_strlen(*insert), \
		second_part, ft_strlen(second_part) + 1);
	free(*host);
	*host = NULL;
	return (ret);
}
