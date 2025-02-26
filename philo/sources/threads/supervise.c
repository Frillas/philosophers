/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:18:20 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/26 12:20:50 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void	verify_philo_end(t_philo *current, t_rules *rules)
{
	int		i;

	i = 0;
	if ((current_time() - current->last_meal_time > rules->time_to_die))
	{
		current->status = DEAD;
		print_status(current);
	}
	if (current->status == DEAD)
	{
		current = current->right;
		while (i < rules->nb_philo)
		{
			current->status = DEAD;
			current = current->right;
			i++;
		}
	}
}

static int	monitor_philo(t_philo *current, t_rules *rules)
{
	long	philo_checked;

	philo_checked = 0;
	while (philo_checked < rules->nb_philo)
	{
		pthread_mutex_lock(&rules->status_lock);
		if (current->status != DEAD)
			verify_philo_end(current, rules);
		if (current->status == DEAD)
		{
			pthread_mutex_unlock(&rules->status_lock);
			return (1);
		}
		pthread_mutex_unlock(&rules->status_lock);
		philo_checked++;
		current = current->right;
	}
	usleep(100);
	return (0);
}

void	*supervise(void *arg)
{
	t_philo	*philo;
	t_philo	*current;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->lst_rules;
	while (1)
	{
		current = philo;
		if (monitor_philo(current, rules) != 0)
			return (NULL);
	}
	return (NULL);
}
