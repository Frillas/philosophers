/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:18:20 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/22 05:32:44 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	verify_philo_end(t_philo *philo, t_rules *rules)
{
	if ((current_time() - philo->last_meal_time > rules->time_to_die))
	{
		philo->status = DEAD;
		print_status(philo);
	}
}

void	*supervise(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->lst_rules;
	while (1)
	{
		sem_wait(philo->lst_rules->sem_status);
		if (philo->status != EAT && philo->status != DEAD)
			verify_philo_end(philo, rules);
		if (philo->status == DEAD)
		{
			sem_post(philo->lst_rules->sem_status);
			break ;
		}
		sem_post(philo->lst_rules->sem_status);
		usleep(1000);
	}
	return (NULL);
}
