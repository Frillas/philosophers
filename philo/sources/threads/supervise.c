/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:18:20 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/16 16:34:00 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void	is_philo_dead(t_philo *current, t_rules *rules)
{
	if (current->meals_eaten == rules->meals_per_philo)
		current->status = DEAD;
	else if ((current_time() - current->last_meal_time > rules->time_to_die))
	{
		current->status = DEAD;
		print_status(current);
	}
}

static int	monitor_philo(t_philo *current, t_rules *rules)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < rules->nb_philo)
	{
		pthread_mutex_lock(&rules->status_lock);
		if (current->status == DEAD)
			j++;
		if (current->status != EAT && current->status != DEAD)
			is_philo_dead(current, rules);
		pthread_mutex_unlock(&rules->status_lock);
		i++;
		current = current->right;
	}
	if (i == j)
		return (1);
	i = 0;
	j = 0;
	usleep(1000);
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
