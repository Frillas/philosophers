/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_philosopher_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:36:02 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/28 12:56:37 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	stop_dinner(t_rules *rules)
{
	sem_wait(rules->sem_end);
	rules->end_dinner = TRUE;
	sem_post(rules->sem_end);
}

static void	*elysium(void *arg)
{
	t_rules	*rules;
	int		i;

	i = 0;
	rules = (t_rules *)arg;
	sem_wait(rules->sem_die);
	sem_wait(rules->sem_end);
	if (rules->end_dinner == TRUE)
	{
		sem_post(rules->sem_end);
		return (NULL);
	}
	sem_post(rules->sem_end);
	while (i < rules->nb_philo)
	{
		kill(rules->fork_id[i], SIGKILL);
		i++;
	}
	if (i == rules->nb_philo)
	{
		stop_dinner(rules);
		sem_post(rules->sem_eat);
	}
	return (NULL);
}

static void	*big_belly(void *arg)
{
	t_rules	*rules;
	int		i;

	rules = (t_rules *)arg;
	i = 0;
	while (i < rules->nb_philo)
	{
		sem_wait(rules->sem_eat);
		sem_wait(rules->sem_end);
		if (rules->end_dinner == TRUE)
		{
			sem_post(rules->sem_end);
			return (NULL);
		}
		sem_post(rules->sem_end);
		i++;
	}
	if (i == rules->nb_philo)
	{
		stop_dinner(rules);
		sem_post(rules->sem_die);
	}
	return (NULL);
}

void	death_and_meal_threads(t_rules *rules)
{
	if (rules->nb_philo > 1)
	{
		pthread_create(&rules->philo_death, NULL, elysium, (void *)rules);
		pthread_detach(rules->philo_death);
		pthread_create(&rules->philo_eat, NULL, big_belly, (void *)rules);
		pthread_detach(rules->philo_eat);
	}
}
