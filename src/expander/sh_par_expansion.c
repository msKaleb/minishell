/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_par_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:56:49 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/16 19:57:39 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief performs parametre expansion ($) in a char *
*/
char	*sh_par_expander(char *tmp, t_list *lenv)
{
	int		i;
	char	*env_var_value;
	char	*quote_state_i;
	char	*token;

	token = ft_strdup(tmp);
	i = -1;
	quote_state_i = NULL;
	get_quote_state_i(token, &quote_state_i);
	while (++i < (int)ft_strlen(token))
	{
		while (token[i] && (token[i] != '$' || quote_state_i[i] == '\''))
			i++;
		if (!token[i] || !token[i + 1] || \
			(!ft_isalpha(token[i + 1]) && !ft_strchr("{?", token[i + 1])))
			break ;
		env_var_value = get_env_var_value(token, i, lenv);
		token = replace_string_chunk(&token, &env_var_value, i);
		get_quote_state_i(token, &quote_state_i);
		i += ft_strlen(env_var_value) - 1;
		free(env_var_value);
	}
	free(quote_state_i);
	return (token);
}

/**
 * @return NULL if special variable not implemented.
 * It's value otherwise.
 * @note mallocs
*/
char	*get_env_sp(char c)
{
	if (c == '?')
		return (ft_itoa(g_error));
	else
		return (NULL);
}

/**
 * @return value of var_name in env_list. NULL if var_name
 * not found.
 * @param env_list is a copy of penv, in linked list form
 * @note frees var_name
*/
char	*get_env(t_list *lenv, char *var_name)
{
	char	*sep;
	char	*temp;

	if (!var_name || !valid_var_name(var_name))
	{
		if (var_name)
			free(var_name);
		return (NULL);
	}
	temp = ft_strjoin(var_name, "=");
	free(var_name);
	while (lenv)
	{
		sep = ft_strchr(lenv->content, '=');
		if (!ft_strncmp(temp, lenv->content, sep - (char *)lenv->content + 1))
		{
			free(temp);
			return (ft_strdup(sep + 1));
		}
		lenv = lenv->next;
	}
	free(temp);
	return (NULL);
}
