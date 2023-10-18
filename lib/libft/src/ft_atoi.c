/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:29:27 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/05 14:23:45 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	int		sign;
	long	res;

	if (!nptr)
		return (0);
	sign = 1;
	res = 0;
	while (*nptr == ' ' || *nptr == '\t' || *nptr == '\v'
		|| *nptr == '\f' || *nptr == '\r' || *nptr == '\n')
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + *nptr - '0';
		nptr++;
	}
	return ((int)(sign * res));
}
