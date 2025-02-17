/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:18:20 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/17 17:31:59 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void	verify_philo_end(t_philo *current, t_rules *rules)
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
	int	philo_checked;
	int	philo_dead;

	philo_checked = 0;
	philo_dead = 0;
	while (philo_checked < rules->nb_philo)
	{
		if (check_mutex_lock(&rules->status_lock) != 0)
			return (1);
		if (current->status == DEAD)
			philo_dead++;
		if (current->status != EAT && current->status != DEAD)
			verify_philo_end(current, rules);
		if (check_mutex_unlock(&rules->status_lock) != 0)
			return (1);
		philo_checked++;
		current = current->right;
	}
	if (philo_checked == philo_dead)
		return (1);
	philo_checked = 0;
	philo_dead = 0;
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
