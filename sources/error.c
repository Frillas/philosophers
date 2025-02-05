/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:42:44 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/05 18:46:54 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

t_bool	is_empty(char *str)
{
	while (*str)
	{
		if (*str != ' ')
			return (FALSE);
		str++;
	}
	write(STDERR_FILENO, "empty argument\n", 15);
	return (TRUE);
}

t_bool	check_arg(t_rules *dinning_rules)
{
	if (dinning_rules->nb_philo == -1)
	{
		write(STDERR_FILENO, "no entry for the number of philo\n", 33);
		return (FALSE);
	}
	else if (dinning_rules->time_to_die == -1)
	{
		write(STDERR_FILENO, "no entry for time to die\n", 25);
		return (FALSE);
	}
	else if (dinning_rules->time_to_eat == -1)
	{
		write(STDERR_FILENO, "ne entry for time to eat\n", 25);
		return (FALSE);
	}
	else if (dinning_rules->time_to_sleep == -1)
	{
		write(STDERR_FILENO, "no entry for time to sleep\n", 27);
		return (FALSE);
	}
	return (TRUE);
}

void	error_msg(char str[], t_rules *dinning_rules)
{
	write(STDERR_FILENO, str, ft_strlen(str)+1);
	dinning_rules->error = TRUE;
}
