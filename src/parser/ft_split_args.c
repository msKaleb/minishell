/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 10:56:14 by msoria-j          #+#    #+#             */
/*   Updated: 2023/09/01 06:51:15 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_wordcount(char *str)
{
	int		isstr;
	int		strnumber;
	int		i;

	i = 0;
	isstr = 0;
	strnumber = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
				i++;
		}
		if (str[i] != 20 && isstr == 0)
		{
			strnumber++;
			isstr = 1;
		}
		if (str[i] == 20)
			isstr = 0;
		i++;
	}
	return (strnumber);
}

static int	get_length(char *str)
{
	int	len;

	len = 0;
	if (str[len] == '\'')
	{
		len++;
		while (str[len] && str[len] != '\'')
			len++;
		if (str[len])
			len++;
		return (len);
	}
	if (str[len] == '"')
	{
		len++;
		while (str[len] && str[len] != '"')
			len++;
		if (str[len])
			len++;
		return (len);
	}
	while (str[len] && str[len] != 20)
		len++;
	return (len);
}

char	**ft_split_args(char *str)
{
	char	**args;
	int		i;
	int		len;

	args = malloc((ft_wordcount(str) + 1) * sizeof(args));
	if (!args)
		return (NULL);
	i = 0;
	while (*str)
	{
		while (*str == 20)
			str++;
		if (*str)
		{
			len = get_length(str);
			args[i] = ft_substr(str, 0, len);
			i++;
			str += len;
		}
	}
	args[i] = NULL;
	return (args);
}
