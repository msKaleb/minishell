/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 13:07:23 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/13 16:35:33 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*copy_list(t_list *src)
{
	t_list	**ret;
	t_list	*temp;

	ret = ft_calloc(1, sizeof(ret));
	while (src)
	{
		temp = ft_lstnew(ft_strdup(src->content));
		ft_lstadd_back(ret, temp);
		src = (src)->next;
	}
	temp = *ret;
	free(ret);
	return (temp);
}

void	print_node_export(void *msg)
{
	char	*sep;

	if (!ft_strncmp(msg, "_=", 2))
		return ;
	sep = ft_strchr(msg, '=');
	ft_fprintf(1, "declare -x ");
	if (sep)
	{
		write(1, msg, sep - (char *)msg);
		ft_fprintf(1, "=\'%s\'\n", sep + 1);
	}
	else
		ft_fprintf(1, "%s\n", msg);
}

/**
 * @brief sorts a linked list in place
*/
void	sort_list(t_list *lst)
{
	t_list	*head;
	t_list	*temp;

	head = lst;
	while (lst->next)
	{
		if (ft_strcmp(lst->content, lst->next->content) > 0)
		{
			temp = lst->next->content;
			lst->next->content = lst->content;
			lst->content = temp;
			lst = head;
			continue ;
		}
		lst = lst->next;
	}
}
