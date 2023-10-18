/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:08:37 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/12 12:42:07 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print(void *msg)
{
	if (*(ft_strchr((char *)msg, '=') + 1) == 0)
		return ;
	ft_fprintf(1, "%s\n", (char *)msg);
}

int	env(t_list **tenvp)
{
	ft_lstiter(*tenvp, &print);
	return (1);
}
