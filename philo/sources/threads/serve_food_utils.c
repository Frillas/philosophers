/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <aroullea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:14:34 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/20 12:13:36 by aroullea         ###   ########.fr       */
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
	return (UNCHANGED);
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

t_status	philo_eat(t_philo *philo)
{
	t_status	status;

	if (check_status(philo, EAT) == ERROR)
	{
		unlock_mutex(&philo->mutex, &philo->left->mutex);
		return (ERROR);
	}
	status = eat_or_sleep(philo->lst_rules->time_to_eat, philo);
	if (status == ERROR)
		unlock_mutex(&philo->mutex, &philo->left->mutex);
	return (status);
}

t_status	philo_sleep(t_philo *philo)
{
	t_status	status;

	if (check_status(philo, SLEEP) == ERROR)
	{
		unlock_mutex(&philo->mutex, &philo->left->mutex);
		return (ERROR);
	}
	if (unlock_mutex(&philo->mutex, &philo->left->mutex) != 0)
		return (ERROR);
	status = eat_or_sleep(philo->lst_rules->time_to_eat, philo);
	return (status);
}

t_status	check_status(t_philo *philo, t_status status)
{
	if (check_mutex_lock(&philo->lst_rules->status_lock) != 0)
		return (ERROR);
	if (philo->status == DEAD)
		status = DEAD;
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		if (philo->status == EAT)
			philo->meals_eaten++;
	}
	if (check_mutex_unlock(&philo->lst_rules->status_lock) != 0)
		return (ERROR);
	return (status);
}
