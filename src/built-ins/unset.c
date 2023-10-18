/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:08:04 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/17 13:21:19 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief removes the environment variables that match
 * any of the args;
*/
int	unset(char **args, t_list **envs_addr, char ***penv_addr)
{
	char	*var_name;

	g_error = B_SUCCESS;
	if (!args || !args[1])
		return (g_error);
	if (args[1][0] == '-')
	{
		ft_perror(B_USAGE, args, "usage: unset [name ...]", 1);
		return (g_error);
	}
	while (*(++args))
	{
		if (read_new_env_var(&var_name, *args))
			continue ;
		remove_matched_nodes(envs_addr, *args);
		free(var_name);
	}
	dbl_free(*penv_addr);
	*penv_addr = fill_penv_dptr(envs_addr);
	return (g_error);
}

static int	is_first_node(t_list **envs_addr, t_list *head, int diff)
{
	if (*envs_addr == head && diff == - '=')
	{
		*envs_addr = (*envs_addr)->next;
		ft_lstdelone(head, free);
		return (1);
	}
	return (0);
}

/**
 * @brief walks trough the list of env_vars, removing the nodes whose
 * content matches arg.
 * The head of env_vars is properly managed.
 * 
*/
void	remove_matched_nodes(t_list **envs_addr, char *arg)
{
	t_list	*temp;
	t_list	*head;
	int		diff;

	head = *envs_addr;
	while (*envs_addr)
	{
		diff = ft_strncmp(arg, (*envs_addr)->content, ft_strlen(arg) + 1);
		if (is_first_node(envs_addr, head, diff))
			return ;
		if (!(*envs_addr)->next)
			break ;
		diff = ft_strncmp(arg, (*envs_addr)->next->content, ft_strlen(arg) + 1);
		if ((*envs_addr)->next && diff == - '=')
		{
			temp = (*envs_addr)->next;
			(*envs_addr)->next = (*envs_addr)->next->next;
			ft_lstdelone(temp, free);
			break ;
		}
		*envs_addr = (*envs_addr)->next;
	}
	*envs_addr = head;
}
