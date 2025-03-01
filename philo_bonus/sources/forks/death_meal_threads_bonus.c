/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_meal_threads_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:36:02 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/01 22:06:31 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	end_diner(t_rules *rules)
{
	sem_wait(rules->sem_end_diner);
	rules->end_dinner = TRUE;
	sem_post(rules->sem_end_diner);
}

static int	check_end_diner(t_rules *rules)
{
	sem_wait(rules->sem_end_diner);
	if (rules->end_dinner == TRUE)
	{
		sem_post(rules->sem_end_diner);
		return (1);
	}
	sem_post(rules->sem_end_diner);
	return (0);
}

static void	*paradise(void *arg)
{
	t_rules	*rules;
	long	fallen_philo;

	fallen_philo = 0;
	rules = (t_rules *)arg;
	sem_wait(rules->sem_die);
	if (check_end_diner(rules) != 0)
		return (NULL);
	while (fallen_philo < rules->created_philo)
	{
		kill(rules->fork_id[fallen_philo], SIGKILL);
		fallen_philo++;
	}
	end_diner(rules);
	sem_post(rules->sem_eat);
	return (NULL);
}

static void	*big_belly(void *arg)
{
	t_rules	*rules;
	long	fed_philo;

	rules = (t_rules *)arg;
	fed_philo = 0;
	while (fed_philo < rules->created_philo)
	{
		sem_wait(rules->sem_eat);
		if (check_end_diner(rules) != 0)
			return (NULL);
		fed_philo++;
	}
	end_diner(rules);
	sem_post(rules->sem_die);
	return (NULL);
}

void	death_and_meal_threads(t_rules *rules)
{
	pthread_t	philo_death;
	pthread_t	philo_eat;

	if (rules->created_philo > 1)
	{
		if (pthread_create(&philo_death, NULL, paradise, (void *)rules) != 0)
		{
			error_msg("thread create error\n", rules);
			return ;
		}
		pthread_detach(philo_death);
		if (pthread_create(&philo_eat, NULL, big_belly, (void *)rules) != 0)
		{
			error_msg("thread create error\n", rules);
			sem_post(rules->sem_die);
			return ;
		}
		pthread_detach(philo_eat);
	}
}
