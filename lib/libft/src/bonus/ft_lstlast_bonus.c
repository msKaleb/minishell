/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 08:09:11 by aperez-m          #+#    #+#             */
/*   Updated: 2023/08/20 11:05:52 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.h"

/**
 * @return the last element of the list
*/
t_list	*ft_lstlast(t_list *lst)
{
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
}
