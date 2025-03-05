/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/05 21:51:54 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	swap(t_philo *philo, pthread_mutex_t **one, pthread_mutex_t **two)
{
	if (philo == philo->left)
	{
		update_status(philo, TAKES_FORK);
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

static void	check_philo_wait(t_philo *philo)
{
	if (philo->lst_rules->time_to_die < 100)
		return ;
	if (philo->meals_eaten == 0 && philo->index % 2 != 0)
		philo_wait(philo->lst_rules, &philo->last_meal_time);
	if (philo->meals_eaten > 0 && philo->lst_rules->nb_philo % 2 == 1)
		philo_wait(philo->lst_rules, &philo->last_meal_time);
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
		check_philo_wait(philo);
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
