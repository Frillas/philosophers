/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/17 06:58:14 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void	swap(t_philo *philo, pthread_mutex_t **one, pthread_mutex_t **two)
{
	*one = &philo->mutex;
	*two = &philo->left->mutex;
	if (philo->index > philo->left->index)
	{
		*one = &philo->left->mutex;
		*two = &philo->mutex;
	}
}

static int	check_status(t_philo *philo, t_status status)
{
	pthread_mutex_lock(&philo->lst_rules->status_lock);
	if (philo->status == DEAD)
	{
		pthread_mutex_unlock(&philo->lst_rules->status_lock);
		return (1);
	}
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		if (philo->status == EAT)
			philo->meals_eaten++;
		pthread_mutex_unlock(&philo->lst_rules->status_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->lst_rules->status_lock);
	return (0);
}

static void	eat_or_sleep(long duration)
{
	long	runtime;
	int		sleeptime;

	runtime = 0;
	sleeptime = 1000;
	if (duration < 1000)
		sleeptime = 100;
	while (runtime < duration)
	{
		usleep(sleeptime);
		runtime += sleeptime;
	}
}

static void	philo_set_state(t_philo *philo)
{
	check_status(philo, EAT);
	eat_or_sleep(philo->lst_rules->time_to_eat);
	philo->last_meal_time = current_time();
	check_status(philo, SLEEP);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(&philo->left->mutex);
	eat_or_sleep(philo->lst_rules->time_to_sleep);
	check_status(philo, THINK);
}

void	*serve_food(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first_mutex;
	pthread_mutex_t	*second_mutex;

	philo = (t_philo *)arg;
	swap(philo, &first_mutex, &second_mutex);
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
