/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:26:16 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/05 12:50:59 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

int	update_status(t_philo *philo, t_status status)
{
	t_rules	*rules;

	rules = philo->lst_rules;
	pthread_mutex_lock(&rules->status_lock);
	if (philo->status == DEAD)
	{
		pthread_mutex_unlock(&rules->status_lock);
		return (1);
	}
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		if (philo->status == EAT)
		{
			philo->last_meal_time = current_time();
			philo->meals_eaten++;
		}
	}
	pthread_mutex_unlock(&rules->status_lock);
	return (0);
}

int	eat_or_sleep(long duration, t_philo *philo)
{
	long	end_time;

	end_time = current_time() + duration;
	while (current_time() < end_time)
	{
		pthread_mutex_lock(&philo->lst_rules->status_lock);
		if (philo->status == DEAD)
		{
			pthread_mutex_unlock(&philo->lst_rules->status_lock);
			return (1);
		}
		pthread_mutex_unlock(&philo->lst_rules->status_lock);
		usleep(100);
	}
	return (0);
}

void	philo_wait(t_philo *philo)
{
	time_t	time_to_wait;
	t_rules	*rules;

	rules = philo->lst_rules;
	time_to_wait = (rules->time_to_eat + rules->time_to_sleep) / 2;
	time_to_wait += current_time();
	while (current_time() < time_to_wait)
	{
		if (((current_time() - philo->last_meal_time) > (rules->time_to_die - 100)))
			break ;
		usleep(300);
	}
}
