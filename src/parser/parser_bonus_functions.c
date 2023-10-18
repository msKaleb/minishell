/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 20:21:39 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/05 20:08:57 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	is_c_operator(char *token, char *command)
{
	if ((ft_strcmp(token, "|") == 0 \
		|| ft_strcmp(token, "||") == 0 \
		|| ft_strcmp(token, "&&") == 0)
		&& ft_strcmp(command, "\024") != 0)
		return (1);
	return (0);
}

int	successful_cmd(t_msh *tm)
{
	if ((tm->and_or == MSH_OR && g_error == 0)
		|| (tm->and_or == MSH_AND && g_error != 0))
		return (0);
	return (1);
}

int	get_loperator(char *str)
{
	if (ft_strcmp(str, "||") == 0)
		return (MSH_OR);
	else if (ft_strcmp(str, "&&") == 0)
		return (MSH_AND);
	else
		return (MSH_PIPE);
}

void	skip_parenthesis(int *index, t_msh *tm)
{
	int	i;
	int	flag;

	i = *index;
	flag = 0;
	if (ft_strcmp(tm->l_exp[i + 1], "(") != 0 || successful_cmd(tm))
		return ;
	while (tm->l_exp[++i])
	{
		if (ft_strcmp(tm->l_exp[i], "(") == 0)
			flag++;
		if (ft_strcmp(tm->l_exp[i], ")") == 0)
			flag--;
		if (flag == 0)
			break ;
	}
	*index = i;
	tm->and_or = get_loperator(tm->l_exp[i + 1]);
	if (tm->and_or != MSH_PIPE)
	{
		*index = i + 1;
		skip_parenthesis(index, tm);
	}
}

/**
 * @brief Decides what to do according to exit status
 * @param index for accessing the last logical operator
*/
void	exec_and_or(char **command, t_msh *tm, int *index)
{
	if (!successful_cmd(tm) || !tm->l_exp[*index + 1])
	{
		free(*command);
		*command = ft_strdup("");
		tm->and_or = get_loperator(tm->l_exp[*index]);
		return ;
	}
	exec_parent(*command, tm);
	restore_io(tm->fdin, tm->fdout);
	tm->fdin = dup(STDIN_FILENO);
	tm->fdout = dup(STDOUT_FILENO);
	if (ft_strcmp(tm->l_exp[*index], "||") == 0)
		tm->and_or = MSH_OR;
	else if (ft_strcmp(tm->l_exp[*index], "&&") == 0)
		tm->and_or = MSH_AND;
	skip_parenthesis(index, tm);
	free(tm->ofile);
	tm->ofile = NULL;
	free(*command);
	*command = ft_strdup("");
}
