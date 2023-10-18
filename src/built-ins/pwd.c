/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:07:17 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/10 19:29:42 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* if (path[2])
	return (ft_perror(errno, path, "Too many arguments", 0)); */

int	pwd(void)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	ft_fprintf(1, "%s\n", cwd);
	return (1);
}
