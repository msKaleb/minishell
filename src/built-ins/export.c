/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:07:45 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/13 17:27:12 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		add_env_var(t_list *envs, char *var_name, char *arg);

/**
 * @param inner_call flags wether export was called from within minishell.
 * Otherwise $? does not reflect the exit status of the last command
 * input by the user.
 * @note ft_strdup(*arg) because *arg is a token of the line read for the 
 * current prompt and will be freed on the next prompt. If a duplicate
 * is not created, the new env var can be overwriten on subsequent prompts.
 * @return 0 on sucess
*/
int	export(char **args, t_list *lenv, char ***envp_addr, int inner_call)
{
	char	*var_name;
	int		ret;

	if (!args[1])
	{
		lenv = copy_list(lenv);
		sort_list(lenv);
		ft_lstiter(lenv, &print_node_export);
		return (ft_lstclear(&lenv, free), g_error = B_SUCCESS, 0);
	}
	ret = 0;
	while (*(++args))
	{
		if (read_new_env_var(&var_name, *args))
		{
			ret = 1;
			continue ;
		}
		add_env_var(lenv, var_name, *args);
	}
	if (!inner_call)
		g_error = ret;
	dbl_free(*envp_addr);
	*envp_addr = fill_penv_dptr(&lenv);
	return (ret);
}

/**
 * @brief assigns the var_name for an arg trying to be exported,
 * appending `=' to a valid var_name
 * @return 0 on sucess
 * @note mallocs
*/
int	read_new_env_var(char **var_name, char *arg)
{
	char	*temp;

	if (ft_strchr(arg, '='))
		*var_name = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
	else
		*var_name = ft_strdup(arg);
	if (!valid_var_name(*var_name))
	{
		ft_fprintf(2, "export: '%s' :is not a valid identifier\n", *var_name);
		free (*var_name);
		return (1);
	}
	temp = *var_name;
	*var_name = ft_strjoin(*var_name, "=");
	free (temp);
	return (0);
}

/**
 * @return 0 if not a valid name for an env_var.
 * Else, its length
*/
int	valid_var_name(char *var_name)
{
	int	i;

	i = 0;
	if (var_name[0] != '_' && (!var_name || !ft_isalpha(var_name[0])))
		return (0);
	while (var_name[++i])
	{
		if (ft_isalnum(var_name[i]) || var_name[i] == '_')
			continue ;
		else
			return (0);
	}
	return (ft_strlen(var_name));
}

/**
 * @brief adds an env_var or changes its value
 * @param var_name the name of the variable to add or whose value to change
 * @param arg either just var_name to add or also its value
*/
static void	add_env_var(t_list *envs, char *var_name, char *arg)
{
	char	*insertion;

	if (!ft_strncmp(var_name, arg, ft_strlen(var_name)))
		insertion = arg;
	else
		insertion = var_name;
	while (envs)
	{
		if (!ft_strncmp((envs)->content, var_name, ft_strlen(var_name)))
		{
			if (!ft_strncmp(arg, var_name, ft_strlen(arg)))
				break ;
			free((envs)->content);
			(envs)->content = ft_strdup(insertion);
			break ;
		}
		if (!envs->next)
		{
			envs->next = ft_lstnew(ft_strdup(insertion));
			break ;
		}
		envs = (envs)->next;
	}
	free(var_name);
}
