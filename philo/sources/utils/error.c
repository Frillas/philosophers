/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:42:44 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/17 17:31:34 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

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

t_bool	check_arg(t_rules *dining_rules)
{
	t_bool		check;

	check = TRUE;
	if (dining_rules->nb_philo == -1 || dining_rules->nb_philo == 0)
	{
		if (dining_rules->nb_philo == -1)
			write(STDERR_FILENO, "no entry for the number of philo\n", 33);
		check = FALSE;
	}
	if (dining_rules->time_to_die == -1)
	{
		write(STDERR_FILENO, "no entry for time to die\n", 25);
		check = FALSE;
	}
	if (dining_rules->time_to_eat == -1)
	{
		write(STDERR_FILENO, "no entry for time to eat\n", 25);
		check = FALSE;
	}
	if (dining_rules->time_to_sleep == -1)
	{
		write(STDERR_FILENO, "no entry for time to sleep\n", 27);
		check = FALSE;
	}
	return (check);
}

void	error_msg(char str[], t_rules *dining_rules)
{
	write(STDERR_FILENO, str, ft_strlen(str) + 1);
	dining_rules->error = TRUE;
}

int	check_mutex_lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex) != 0)
	{
		write(STDERR_FILENO, "Failed to lock mutex\n", 21);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_mutex_unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex) != 0)
	{
		write(STDERR_FILENO, "Failed to unlock mutex\n", 21);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
