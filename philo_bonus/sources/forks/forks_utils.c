/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:32:34 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/21 16:35:33 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

int	check_status(t_philo *philo, t_status status)
{
	sem_wait(philo->lst_rules->sem_print);
	if (philo->status == DEAD)
	{
		sem_post(philo->lst_rules->sem_print);
		return (1);
	}
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		if (philo->status == EAT)
			philo->meals_eaten++;
		sem_post(philo->lst_rules->sem_print);
		return (0);
	}
	sem_post(philo->lst_rules->sem_print);
	return (0);
}

void	eat_or_sleep(long long duration, t_philo *philo)
{
	long	runtime;

	runtime = 0;
	duration *= 1000;
	while (runtime < duration)
	{
		sem_wait(philo->lst_rules->sem_print);
		if (philo->status == DEAD)
		{
			sem_post(philo->lst_rules->sem_print);
			return ;
		}
		sem_post(philo->lst_rules->sem_print);
		usleep(10000);
		runtime += 10000;
	}
}
