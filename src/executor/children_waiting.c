/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_waiting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 19:17:12 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/14 11:24:09 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lst_wait_children(void *msg)
{
	char	*str;
	int		n;

	str = (char *)msg;
	n = ft_atoi(str);
	waitpid(n, NULL, 0);
}

void	wait_for_children(t_msh *tm)
{
	t_list	*tmp;

	ft_lstiter(*tm->pids, &lst_wait_children);
	ft_lstclear(tm->pids, &free);
	while (*tm->pids)
	{
		tmp = *tm->pids;
		*tm->pids = (*tm->pids)->next;
		free(tmp);
	}
	*tm->pids = NULL;
}

void	add_fork_id(pid_t id, t_msh *tm)
{
	t_list	*tmp;
	char	*fid;

	fid = ft_itoa(id);
	tmp = ft_lstnew(fid);
	ft_lstadd_back(tm->pids, tmp);
}
