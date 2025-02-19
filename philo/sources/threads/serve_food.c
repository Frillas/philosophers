/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:22:45 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/19 11:55:10 by aroullea         ###   ########.fr       */
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

static int	unlock_mutex(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2)
{
	int	error;

	error = 0;
	if (check_mutex_unlock(mutex1) != 0)
		error = 1;
	if (check_mutex_unlock(mutex2) != 0)
		error = 1;
	return (error);
}

static t_status	philo_set_state(t_philo *philo)
{
	if (check_status(philo, EAT) == ERROR)
	{
		unlock_mutex(&philo->mutex, &philo->left->mutex);
		return (ERROR);
	}
	eat_or_sleep(philo->lst_rules->time_to_eat);
	philo->last_meal_time = current_time();
	if (check_status(philo, SLEEP) == ERROR)
	{
		unlock_mutex(&philo->mutex, &philo->left->mutex);
		return (ERROR);
	}
	if (unlock_mutex(&philo->mutex, &philo->left->mutex) != 0)
		return (ERROR);
	eat_or_sleep(philo->lst_rules->time_to_sleep);
	if (check_status(philo, THINK) == ERROR)
		return (ERROR);
	return (THINK);
}

static t_status	philo_take_fork(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	t_status	status;

	if (check_mutex_lock(first) != 0)
		return (ERROR);
	status = check_status(philo, TAKES_FORK);
	if (status == ERROR)
	{
		check_mutex_unlock(first);
		return (ERROR);
	}
	if (check_mutex_lock(second) != 0)
	{
		check_mutex_unlock(first);
		return (ERROR);
	}
	status = check_status(philo, TAKES_FORK);
	if (status == ERROR || status == DEAD)
	{
		if (unlock_mutex(first, second) != 0 || status == ERROR)
			return (ERROR);
	}
	return (status);
}

void	*serve_food(void *arg)
{
	t_status		*status;
	t_philo			*philo;
	pthread_mutex_t	*first_mutex;
	pthread_mutex_t	*second_mutex;

	philo = (t_philo *)arg;
	status = (t_status *)malloc(sizeof(t_status));
	if (status == NULL)
		return (NULL);
	if (swap(philo, &first_mutex, &second_mutex) != 0)
		return (NULL);
	while (1)
	{
		*status = check_status(philo, UNCHANGED);
		if (*status == ERROR || *status == DEAD)
			return ((int *)status);
		*status = philo_take_fork(philo, first_mutex, second_mutex);
		if (*status == ERROR || *status == DEAD)
			return ((int *)status);
		*status = philo_set_state(philo);
		if (*status == ERROR || *status == DEAD)
			return ((int *)status);
	}
	return (NULL);
}
