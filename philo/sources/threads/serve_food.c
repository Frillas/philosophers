/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:22:45 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/20 14:43:27 by aroullea         ###   ########.fr       */
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

static t_status	philo_set_state(t_philo *philo)
{
	t_status	status;

	status = philo_eat(philo);
	if (status == ERROR || status == DEAD)
		return (status);
	philo->last_meal_time = current_time();
	status = philo_sleep(philo);
	if (status == ERROR || status == DEAD)
		return (status);
	if (check_status(philo, THINK) == ERROR)
		return (ERROR);
	return (THINK);
}

static t_status	t_fork(t_philo *philo, pthread_mutex_t *m1, pthread_mutex_t *m2)
{
	t_status	status;

	if (check_mutex_lock(m1) != 0)
		return (ERROR);
	status = check_status(philo, TAKES_FORK);
	if (status == ERROR || (check_mutex_lock(m2) != 0))
	{
		check_mutex_unlock(m1);
		return (ERROR);
	}
	status = check_status(philo, TAKES_FORK);
	if (status == ERROR || status == DEAD)
	{
		if ((unlock_mutex(m1, m2) != 0) || status == ERROR)
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
		*status = t_fork(philo, first_mutex, second_mutex);
		if (*status == ERROR || *status == DEAD)
			return ((int *)status);
		*status = philo_set_state(philo);
		if (*status == ERROR || *status == DEAD)
			return ((int *)status);
	}
	return (NULL);
}
