/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_meal_threads_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:36:02 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/02 08:28:38 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	end_diner(t_rules *rules)
{
	sem_wait(rules->sem_end_diner);
	rules->end_dinner = TRUE;
	sem_post(rules->sem_end_diner);
}

static t_bool	get_end_diner_status(t_rules *rules)
{
	int	status;

	sem_wait(rules->sem_end_diner);
	status = rules->end_dinner;
	sem_post(rules->sem_end_diner);
	return (status);
}

static void	*terminate_philo(void *arg)
{
	t_rules	*rules;
	long	fallen_philo;

	fallen_philo = 0;
	rules = (t_rules *)arg;
	sem_wait(rules->sem_die);
	if (get_end_diner_status(rules) == TRUE)
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
		if (get_end_diner_status(rules) == TRUE)
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

	if (rules->created_philo <= 1)
		return ;
	if (pthread_create(&philo_death, NULL, terminate_philo, (void *)rules) != 0)
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
