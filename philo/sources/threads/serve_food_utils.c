/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:26:16 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/06 03:49:03 by aroullea         ###   ########.fr       */
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
		usleep(1000);
	}
	return (0);
}

void	philo_think(t_rules *rules, time_t *last_meal)
{
	time_t	duration;

	duration = rules->time_to_think + current_time();
	while (current_time() < duration)
	{
		if ((current_time() - *last_meal) > rules->is_hungry)
			break ;
		usleep(1000);
	}
}
