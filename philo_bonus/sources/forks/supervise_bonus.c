/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:18:20 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/09 15:20:19 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	*all_meals_eaten(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->lst_rules->sem_meals_eaten);
	philo->lst_rules->end_dinner = TRUE;
	return (NULL);
}

static int	verify_philo_end(t_philo *philo, t_rules *rules)
{
	if ((current_time() - philo->last_meal_time > rules->time_to_die))
	{
		philo->status = DEAD;
		print_status(philo);
		sem_post(rules->sem_die);
		return (1);
	}
	return (0);
}

void	*supervise(void *arg)
{
	t_philo		*philo;
	t_rules		*rules;
	pthread_t	satiated;

	philo = (t_philo *)arg;
	rules = philo->lst_rules;
	if (pthread_create(&satiated, NULL, all_meals_eaten, (void *)philo) != 0)
		error_msg("thread create in supervisor error\n", rules);
	while (1)
	{
		sem_wait(philo->lst_rules->sem_status);
		if (rules->end_dinner == TRUE)
		{
			sem_post(philo->lst_rules->sem_status);
			break ;
		}
		if (verify_philo_end(philo, rules) != 0)
			break ;
		sem_post(philo->lst_rules->sem_status);
		usleep(500);
	}
	return (NULL);
}
