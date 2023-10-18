/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_par_expansion_parsing.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:56:49 by aperez-m     +#+  #+#    #+#             */
/*   Updated: 2023/10/14 11:17:42 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief gets the name of the environement variable to expand
 * inmediately after d_pos, and its value from env_list.
 * NULL if no match found.
 * @note mallocs
*/
char	*get_env_var_value(char *lexed_line, int d_pos, t_list *lenv)
{
	char	*name;
	char	*value;
	char	*temp;

	value = NULL;
	if (ft_strchr("?$!#0", *(lexed_line + d_pos + 1)))
		return (get_env_sp(*(lexed_line + d_pos + 1)));
	name = get_env_var_name(lexed_line, d_pos);
	if (!name)
		return (NULL);
	if (*(lexed_line + d_pos + 1) == '{' && !valid_var_name(name))
	{
		temp = ft_strdup("\"bash: ${");
		value = ft_strjoin(temp, name);
		free(temp);
		temp = value;
		value = ft_strjoin(temp, "}: bad sustitution\"");
		free(temp);
		free(name);
		return (value);
	}
	value = get_env(lenv, name);
	return (value);
}

/**
 * @return "" if no variable name found. If it returned
 * NULL getenv(NULL) would segfault
 * @note mallocs
*/
char	*get_env_var_name(char *lexed_line, int d_pos)
{
	int		size;
	char	*start;
	char	*end;
	int		i;

	if (lexed_line[d_pos + 1] == '{')
	{
		start = lexed_line + d_pos + 2;
		end = ft_strchr(lexed_line + d_pos, '}') - 1;
	}
	else
	{
		start = lexed_line + d_pos + 1;
		i = 0;
		while (ft_isalnum(lexed_line[i + d_pos + 1]) || \
			lexed_line[i + d_pos + 1] == '_')
			++i;
		end = lexed_line + d_pos + i;
	}
	size = (end - start) + 1;
	if (size == 1 && *start == '{')
		return (NULL);
	return (ft_substr(start, 0, size));
}

/**
 * @return where the name of the variable to be replaced
 * starts after the $, as it can be braced with {}
*/
char	*get_var_start(char *token, size_t d_pos)
{
	if (ft_isalpha(token[d_pos + 1]))
		return (token + d_pos + 1);
	else if (token[d_pos + 1] == '{')
		return (token + d_pos + 2);
	else
		return (token + d_pos);
}

/**
 * @brief returns where the name of the variable to be replaced
 * ends after the $, as it can be braced with {}
*/
char	*get_var_end(char *token, size_t d_pos)
{
	char	*ret;
	char	*brace_state_i;

	brace_state_i = get_brace_state_i(token);
	ret = get_parameter_end(token, d_pos);
	if (!ret)
		ret = token + ft_strlen(token) - 1;
	if (brace_state_i[d_pos + 1] == '{')
		ret -= 1;
	free(brace_state_i);
	return (ret);
}

/**
 * @brief returns where the chunk to be replaced 
 * ends after the $, as it can be braced with {}
*/
char	*get_parameter_end(char *token, int d_pos)
{
	int		i;
	char	*brace_state_i;
	char	*ret;

	if (token[d_pos + 1] == '?')
		return (token + d_pos + 1);
	brace_state_i = get_brace_state_i(token);
	if (!brace_state_i)
		return (NULL);
	if (brace_state_i[d_pos + 1] == '{')
	{
		ret = ft_strchr(brace_state_i + d_pos + 1, '_');
		ret += token - brace_state_i;
		free(brace_state_i);
		return (ret);
	}
	i = 1;
	while (ft_isalnum(token[d_pos + i]) || token[d_pos + i] == '_')
		i++;
	free(brace_state_i);
	return (token + d_pos + i - 1);
}
