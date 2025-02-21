/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:18:20 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/21 11:46:13 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

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
		sem_wait(philo->lst_rules->sem_print);
		if (philo->status != EAT && philo->status != DEAD)
			verify_philo_end(philo, rules);
		if (philo->status == DEAD)
		{
			sem_post(philo->lst_rules->sem_print);
			break;
		}
		sem_post(philo->lst_rules->sem_print);
		usleep(1000);
	}
	return (NULL);
}
