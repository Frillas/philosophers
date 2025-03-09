/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/09 11:14:53 by aroullea         ###   ########.fr       */
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
	*one = &philo->fork_mutex;
	*two = &philo->left->fork_mutex;
	if (philo->index > philo->left->index)
	{
		*one = &philo->left->fork_mutex;
		*two = &philo->fork_mutex;
	}
	return (0);
}

static void	check_philo_think(t_philo *philo)
{
	if (philo->lst_rules->time_to_die < 100)
		return ;
	if (philo->meals_eaten == 0 && philo->index % 2 != 0)
		philo_think(philo->lst_rules, &philo->last_meal_time);
	if (philo->meals_eaten > 0 && philo->lst_rules->nb_philo % 2 != 0)
		philo_think(philo->lst_rules, &philo->last_meal_time);
}

static void	philo_set_state(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->lst_rules;
	update_status(philo, EAT);
	if (eat_or_sleep(rules->time_to_eat, philo) != 0)
	{
		pthread_mutex_unlock(&philo->fork_mutex);
		pthread_mutex_unlock(&philo->left->fork_mutex);
		return ;
	}
	pthread_mutex_lock(&rules->meals_lock);
	if (philo->meals_eaten == rules->meals_per_philo)
		rules->meals_count++;
	pthread_mutex_unlock(&rules->meals_lock);
	update_status(philo, SLEEP);
	pthread_mutex_unlock(&philo->fork_mutex);
	pthread_mutex_unlock(&philo->left->fork_mutex);
	if (eat_or_sleep(rules->time_to_sleep, philo) != 0)
		return ;
	update_status(philo, THINK);
}

static void	forks(t_philo *philo, pthread_mutex_t **one, pthread_mutex_t **two)
{
	while (1)
	{
		if (update_status(philo, UNCHANGED) || check_meals(philo->lst_rules))
			break ;
		check_philo_think(philo);
		pthread_mutex_lock(*one);
		if (check_meals(philo->lst_rules))
		{
			pthread_mutex_unlock(*one);
			break ;
		}
		update_status(philo, TAKES_FORK);
		pthread_mutex_lock(*two);
		if (update_status(philo, TAKES_FORK) == 1)
		{
			pthread_mutex_unlock(*one);
			pthread_mutex_unlock(*two);
			break ;
		}
		philo_set_state(philo);
	}
}

void	*serve_food(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first_mutex;
	pthread_mutex_t	*second_mutex;

	philo = (t_philo *)arg;
	if (swap(philo, &first_mutex, &second_mutex) != 0)
		return (NULL);
	forks(philo, &first_mutex, &second_mutex);
	return (NULL);
}
