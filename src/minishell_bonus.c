/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 16:59:49 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/12 13:58:40 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	parenthesis_status(int flag)
{
	if (flag != 0)
	{
		g_error = B_GENERIC;
		printf("Parenthesis not closed\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_parenthesis(char *line_read)
{
	char	*quote_state_i;
	int		flag;
	int		i;

	if (!line_read)
		return (EXIT_SUCCESS);
	i = -1;
	flag = 0;
	quote_state_i = NULL;
	get_quote_state_i(line_read, &quote_state_i);
	while (line_read[++i])
	{
		if (flag < 0)
			break ;
		if (line_read[i] == '(' && quote_state_i[i] == '_')
			flag++;
		if (line_read[i] == ')' && quote_state_i[i] == '_')
			flag--;
	}
	free(quote_state_i);
	return (parenthesis_status(flag));
}

void	minishell(t_msh *tm)
{
	int		i;
	char	*temp;
	char	*temp2;

	temp2 = ft_strdup("PATH");
	temp = get_env(*tm->tenvp, temp2);
	tm->paths = ft_split(temp, ':');
	free(temp);
	tm->line_read = ft_readline(tm);
	lexer(&tm->line_read);
	if (check_parenthesis(tm->line_read) != EXIT_SUCCESS)
		free(tm->line_read);
	else if (tm->line_read)
	{
		tm->line_read_split = ft_split_msh(tm->line_read, ' ');
		tm->l_exp = line_expander(&tm->line_read_split, *tm->tenvp);
		i = -1;
		is_quoted_array(tm);
		while (tm->l_exp[++i])
			quote_remove(tm->l_exp[i]);
		if (tm->l_exp[0] && tm->l_exp[0][0])
			get_command_table(tm);
		free_on_loop(tm);
	}
	dbl_free(tm->paths);
}
