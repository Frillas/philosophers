/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:18:20 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/16 15:16:19 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	check_state(t_philo *current, t_rules *rules)
{
	if ((current_time() - current->last_meal_time > rules->time_to_die))
	{
		current->status = DEAD;
		print_status(current);
	}
}

void	*supervise(void *arg)
{
	t_philo	*philo;
	t_philo	*current;
	t_rules	*rules;
	int		i;
	int		j;

	philo = (t_philo *)arg;
	rules = philo->lst_rules;
	i = 0;
	j = 0;
	while (1)
	{
		current = philo;
		while (i < rules->nb_philo)
		{
			pthread_mutex_lock(&rules->status_lock);
			if (current->status == DEAD)
				j++;
			if (current->status != EAT && current->status != DEAD)
				check_state(current, rules);
			pthread_mutex_unlock(&rules->status_lock);
			i++;
			current = current->right;
		}
		if (i == j)
			return (NULL);
		i = 0;
		j = 0;
		usleep(1000);
	}
	return (NULL);
}
