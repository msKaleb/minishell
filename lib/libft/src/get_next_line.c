/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 10:32:45 by aperez-m          #+#    #+#             */
/*   Updated: 2023/05/27 08:00:51 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.h"

char	*get_next_line(int fd)
{
	static char	*temp_stash;
	char		*ret;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	temp_stash = fill_temp_stash(fd, temp_stash);
	if (temp_stash == NULL)
		return (NULL);
	ret = fill_ret(temp_stash);
	temp_stash = trim_temp_stash(temp_stash);
	return (ret);
}
