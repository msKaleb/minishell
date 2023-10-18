/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 19:48:00 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/04 17:21:07 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*list_to_string(t_list *lret)
{
	char	*ret;
	char	*temp;

	ret = ft_strdup("");
	while (lret)
	{
		temp = ret;
		ret = ft_strjoin(ret, lret->content);
		free(temp);
		temp = ret;
		ret = ft_strjoin(ret, " ");
		free (temp);
		lret = lret->next;
	}
	return (ret);
}

/**
 * @return a char * with all the filenames that match 
 * pattern, separated by spaces.
*/
char	*fn_expander(char *pattern)
{
	DIR				*d;
	struct dirent	*dir;
	char			*ret;
	t_list			*lret;

	d = opendir(".");
	lret = NULL;
	if (!d)
		exit(1);
	dir = readdir(d);
	while (dir != NULL)
	{
		if ((dir->d_type == DT_REG || dir->d_type == DT_DIR) \
			&& glob_engine(pattern, dir->d_name))
			ft_lstadd_back(&lret, ft_lstnew(ft_strdup(dir->d_name)));
		dir = readdir(d);
	}
	closedir(d);
	if (!lret)
		return (ft_strdup(pattern));
	sort_list(lret);
	ret = list_to_string(lret);
	ft_lstclear(&lret, free);
	return (ret);
}

int	glob_engine(char *pattern, char *target)
{
	char	**pattern_list;
	int		i;
	char	*match;

	i = -1;
	pattern_list = ft_split_msh(pattern, '*');
	while (pattern_list[++i])
	{
		match = ft_strnstr(target, pattern_list[i], ft_strlen(target));
		if (!match)
			break ;
		if (i || (!i && pattern[0] == '*' && match != target) || \
			(!i && match == target))
			target = match + ft_strlen(pattern_list[i]);
		else
			break ;
	}
	if (!pattern_list[i] && (!*target || \
		(*target && (pattern[ft_strlen(pattern) - 1] == '*'))))
		return (dbl_free(pattern_list), 1);
	return (dbl_free(pattern_list), 0);
}
