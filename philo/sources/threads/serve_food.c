/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/18 09:39:15 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	swap(t_philo *philo, pthread_mutex_t **one, pthread_mutex_t **two)
{
	if (philo == philo->left)
		return (1);
	*one = &philo->mutex;
	*two = &philo->left->mutex;
	if (philo->index > philo->left->index)
	{
		*one = &philo->left->mutex;
		*two = &philo->mutex;
	}
	return (0);
}

static t_status	check_status(t_philo *philo, t_status status)
{
	if (check_mutex_lock(&philo->lst_rules->status_lock) != 0)
		return (ERROR);
	if (philo->status == DEAD)
	{
		if (check_mutex_unlock(&philo->lst_rules->status_lock) != 0)
			return (ERROR);
		return (DEAD);
	}
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		if (philo->status == EAT)
			philo->meals_eaten++;
		if (check_mutex_unlock(&philo->lst_rules->status_lock) != 0)
			return (ERROR);
		return (status);
	}
	if (check_mutex_unlock(&philo->lst_rules->status_lock) != 0)
		return (ERROR);
	return (status);
}

static void	eat_or_sleep(long long duration)
{
	long	runtime;

	runtime = 0;
	duration *= 1000;
	while (runtime < duration)
	{
		usleep(10000);
		runtime += 10000;
	}
}

static int	philo_set_state(t_philo *philo)
{
	t_status	status;

	if (check_status(philo, EAT) == ERROR)
	{
		if (check_mutex_unlock(&philo->mutex) != 0)
		{
			if (check_mutex_unlock(&philo->left->mutex) != 0)
			return (1);
		}
	}
	eat_or_sleep(philo->lst_rules->time_to_eat);
	philo->last_meal_time = current_time();
	status = check_status(philo, SLEEP);
	if (check_mutex_unlock(&philo->mutex) != 0)
	{
		if (check_mutex_unlock(&philo->left->mutex) != 0)
		return (1);
	}
	if (check_mutex_unlock(&philo->left->mutex) != 0)
		return (1);
	if (status == ERROR)
		return (1);
	eat_or_sleep(philo->lst_rules->time_to_sleep);
	if (check_status(philo, THINK) == ERROR)
		return (1);
	return (0);
}

void	*serve_food(void *arg)
{
	t_status		status;
	t_philo			*philo;
	pthread_mutex_t	*first_mutex;
	pthread_mutex_t	*second_mutex;

	philo = (t_philo *)arg;
	if (swap(philo, &first_mutex, &second_mutex) != 0)
		return (NULL);
	while (1)
	{
		status = check_status(philo, UNCHANGED);
		if (status == DEAD || status == ERROR)
			return ((int *)status);
		if (check_mutex_lock(first_mutex) != 0)
			return ((int *)1);
		if (check_status(philo, TAKES_FORK) == ERROR)
			return ((int *)status);
		if (check_mutex_lock(second_mutex) != 0)
		{
			if (check_mutex_unlock(first_mutex) != 0)
				return ((int *)1);
		}
		status = check_status(philo, TAKES_FORK);
		if (status == DEAD || status == ERROR)
		{
			if (check_mutex_unlock(first_mutex) != 0)
			{
				if (check_mutex_unlock(second_mutex) != 0)
					return ((int *)1);
			}
			if (check_mutex_unlock(second_mutex) != 0)
				return ((int *)1);
			return (NULL);
		}
		if (philo_set_state(philo) != 0)
			return (NULL);
	}
	return (NULL);
}
