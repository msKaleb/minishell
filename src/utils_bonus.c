/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 15:47:01 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/09 18:56:48 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dbl_free(char **ptr)
{
	int	i;

	if (!ptr)
		return ;
	i = -1;
	while (ptr[++i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
	}
	free(ptr);
	ptr = NULL;
}

/**
 * @note frees s1
*/
char	*path_join(char *s1, char *s2)
{
	char	*s;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2);
	s = NULL;
	s = malloc(len + 1);
	if (s == NULL)
		return (NULL);
	ft_strlcpy(s, s1, len + 1);
	ft_strlcat(s, s2, len + 1);
	if (ft_strlen(s1) > 0 || s1[0] == 0)
		free(s1);
	return (s);
}

/**
 * @note mallocs
*/
char	*create_prompt(t_msh *tm)
{
	char	*prompt;
	char	cwd[PATH_MAX];
	char	*pcwd;
	char	*temp;
	char	*temp2;

	getcwd(cwd, sizeof(cwd));
	pcwd = ft_strrchr(cwd, '/') + 1;
	temp = ft_strdup("HOME");
	temp2 = get_env(*tm->tenvp, temp);
	if (!ft_strcmp(cwd, temp2))
		pcwd = "~";
	else if (ft_strcmp(cwd, "/") == 0)
		pcwd = "/";
	free(temp2);
	prompt = ft_strdup("\001\x1b[1;34m\002[");
	temp = ft_strdup("USER");
	temp2 = get_env(*tm->tenvp, temp);
	prompt = path_join(prompt, temp2);
	free(temp2);
	prompt = path_join(prompt, "@minishell \001\x1b[1;32m\002");
	prompt = path_join(prompt, pcwd);
	prompt = path_join(prompt, "\001\x1b[1;34m\2]$\001\x1b[0m\002 ");
	return (prompt);
}

int	is_built_in(char *cmd)
{
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

void	free_on_loop(t_msh *tm)
{
	if (tm->line_read_split)
		dbl_free(tm->line_read_split);
	if (tm->l_exp)
		dbl_free(tm->l_exp);
	free(tm->line_read);
	free(tm->is_quoted);
}
