/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <aroullea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:14:34 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/19 15:29:22 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

t_status	eat_or_sleep(long long duration, t_philo *philo)
{
	long		runtime;

	runtime = 0;
	duration *= 1000;
	while (runtime < duration)
	{
		if (check_mutex_lock(&philo->lst_rules->status_lock) != 0)
			return (ERROR);
		if (philo->status == DEAD)
		{
			check_mutex_unlock(&philo->lst_rules->status_lock);
			return (DEAD);
		}
		if (check_mutex_unlock(&philo->lst_rules->status_lock) != 0)
			return (ERROR);
		usleep(10000);
		runtime += 10000;
	}
	return (0);
}

int	unlock_mutex(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2)
{
	int	error;

	error = 0;
	if (check_mutex_unlock(mutex1) != 0)
		error = 1;
	if (check_mutex_unlock(mutex2) != 0)
		error = 1;
	return (error);
}
