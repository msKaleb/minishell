/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 13:28:07 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/12 16:49:44 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

static void	default_env(t_list **lst)
{
	char	cwd[PATH_MAX];
	char	*pwd;
	t_list	*tmp;

	getcwd(cwd, sizeof(cwd));
	pwd = ft_strdup("PWD=");
	pwd = path_join(pwd, cwd);
	tmp = ft_lstnew(ft_strdup((char *)pwd));
	ft_lstadd_back(lst, tmp);
	free(pwd);
	pwd = ft_strdup("PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin");
	tmp = ft_lstnew(ft_strdup((char *)pwd));
	ft_lstadd_back(lst, tmp);
	free(pwd);
}

void	shlvl_var(t_list **lst)
{
	t_list	*tmp;
	int		shlvl;
	char	*shlvl_char;

	tmp = *lst;
	while (*lst)
	{
		if (ft_strncmp((*lst)->content, "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi((*lst)->content + 6);
			shlvl_char = ft_itoa(shlvl + 1);
			free((*lst)->content);
			(*lst)->content = ft_strdup("SHLVL=");
			(*lst)->content = path_join((*lst)->content, shlvl_char);
			free(shlvl_char);
		}
		*lst = (*lst)->next;
	}
	*lst = tmp;
}

t_list	**fill_penv_list(char **penv)
{
	t_list	**lst;
	t_list	*tmp;
	int		i;

	lst = ft_calloc(1, sizeof(**lst));
	if (!penv[0])
	{
		default_env(lst);
		return (lst);
	}
	i = -1;
	while (penv[++i])
	{
		tmp = ft_lstnew(ft_strdup((char *)penv[i]));
		ft_lstadd_back(lst, tmp);
	}
	shlvl_var(lst);
	return (lst);
}

char	**fill_penv_dptr(t_list **lst)
{
	t_list	*tmp;
	char	**penv;
	int		size;
	int		i;

	i = 0;
	tmp = *lst;
	size = ft_lstsize((*lst));
	penv = malloc((size + 1) * sizeof(penv));
	while ((*lst))
	{
		penv[i] = ft_strdup((char *)(*lst)->content);
		(*lst) = (*lst)->next;
		i++;
	}
	penv[i] = NULL;
	*lst = tmp;
	return (penv);
}
