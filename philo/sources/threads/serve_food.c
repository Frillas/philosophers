/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/24 10:22:24 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	check_status(t_philo *philo, t_status status)
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
			philo->meals_eaten++;
	}
	pthread_mutex_unlock(&rules->status_lock);
	return (0);
}

static int	swap(t_philo *philo, pthread_mutex_t **one, pthread_mutex_t **two)
{
	if (philo == philo->left)
	{
		check_status(philo, TAKES_FORK);
		return (1);
	}
	*one = &philo->mutex;
	*two = &philo->left->mutex;
	if (philo->index > philo->left->index)
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
	check_status(philo, EAT);
	philo->last_meal_time = current_time();
	if (eat_or_sleep(rules->time_to_eat, philo) != 0)
	{
		pthread_mutex_unlock(&philo->mutex);
		pthread_mutex_unlock(&philo->left->mutex);
		return ;
	}
	check_status(philo, SLEEP);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(&philo->left->mutex);
	if (eat_or_sleep(rules->time_to_sleep, philo) != 0)
		return ;
	check_status(philo, THINK);
	pthread_mutex_lock(&rules->status_lock);
	if (philo->meals_eaten == rules->meals_per_philo)
		philo->status = DEAD;
	pthread_mutex_unlock(&rules->status_lock);
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
		if (check_status(philo, UNCHANGED) == 1)
			return (NULL);
		pthread_mutex_lock(first_mutex);
		check_status(philo, TAKES_FORK);
		pthread_mutex_lock(second_mutex);
		if (check_status(philo, TAKES_FORK) == 1)
		{
			pthread_mutex_unlock(first_mutex);
			pthread_mutex_unlock(second_mutex);
			return (NULL);
		}
		philo_set_state(philo);
	}
	return (NULL);
}
