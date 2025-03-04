/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/04 11:57:01 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	update_status(t_philo *philo, t_status status)
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

static int	swap(t_philo *philo, pthread_mutex_t **one, pthread_mutex_t **two)
{
	if (philo == philo->left)
	{
		update_status(philo, TAKES_FORK);
		return (1);
	}
	if (philo->index % 2 != 0)
	{
		*one = &philo->mutex;
		*two = &philo->left->mutex;
	}
	else
	{
		*one = &philo->left->mutex;
		*two = &philo->mutex;
	}
	return (0);
}

static int	eat_or_sleep(long duration, t_philo *philo)
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

static void	philo_set_state(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->lst_rules;
	update_status(philo, EAT);
	if (eat_or_sleep(rules->time_to_eat, philo) != 0)
	{
		pthread_mutex_unlock(&philo->mutex);
		pthread_mutex_unlock(&philo->left->mutex);
		return ;
	}
	update_status(philo, SLEEP);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(&philo->left->mutex);
	if (eat_or_sleep(rules->time_to_sleep, philo) != 0)
		return ;
	update_status(philo, THINK);
	pthread_mutex_lock(&rules->status_lock);
	if (philo->meals_eaten == rules->meals_per_philo)
		philo->status = DEAD;
	pthread_mutex_unlock(&rules->status_lock);
}

static void	philo_think(t_philo *philo)
{
	time_t	time_to_think;
	t_rules	*rules;

	rules = philo->lst_rules;
	time_to_think = current_time() + (rules->time_to_eat + rules->time_to_sleep) / 4;
	while (current_time() < time_to_think)
		usleep(300);
}

void	*serve_food(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first_mutex;
	pthread_mutex_t	*second_mutex;

	philo = (t_philo *)arg;
	if (swap(philo, &first_mutex, &second_mutex) != 0)
		return (NULL);
	while (1)
	{
		if (update_status(philo, UNCHANGED) == 1)
			return (NULL);
		if (philo->meals_eaten == 0 && philo->index % 2 == 0)
			usleep(300);
		if (philo->lst_rules->nb_philo % 2 != 0 && philo->meals_eaten > 0)
			philo_think(philo);
		pthread_mutex_lock(first_mutex);
		update_status(philo, TAKES_FORK);
		pthread_mutex_lock(second_mutex);
		if (update_status(philo, TAKES_FORK) == 1)
		{
			pthread_mutex_unlock(first_mutex);
			pthread_mutex_unlock(second_mutex);
			return (NULL);
		}
		philo_set_state(philo);
	}
	return (NULL);
}
