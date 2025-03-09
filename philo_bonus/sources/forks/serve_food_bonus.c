/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:54:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/09 14:37:46 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	check_philo_think(t_philo *philo)
{
	if (philo->lst_rules->time_to_die < 100)
		return ;
	if (philo->meals_eaten == 0 && philo->index % 2 == 0)
		philo_think(philo->lst_rules, &philo->last_meal_time);
	if (philo->meals_eaten > 0 && philo->lst_rules->nb_philo % 2 != 0)
		philo_think(philo->lst_rules, &philo->last_meal_time);
}

static void	philo_cycle(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->lst_rules;
	update_status(philo, EAT);
	if (wait_with_death_check(rules->time_to_eat, philo) == DEAD)
	{
		sem_post(rules->sem_fork);
		sem_post(rules->sem_fork);
		return ;
	}
	sem_wait(rules->sem_status);
	if (philo->meals_eaten == rules->meals_per_philo)
		sem_post(rules->sem_eat);
	sem_post(rules->sem_status);
	update_status(philo, SLEEP);
	sem_post(rules->sem_fork);
	sem_post(rules->sem_fork);
	if (wait_with_death_check(rules->time_to_sleep, philo) == DEAD)
		return ;
	update_status(philo, THINK);
}

void	start_routine( t_rules *rules, t_philo *philo, pid_t *fork_id)
{
	while (1)
	{
		sem_wait(rules->sem_status);
		if (rules->end_dinner == TRUE)
		{
			sem_post(rules->sem_status);
			free_exit(philo, fork_id, 0);
		}
		sem_post(rules->sem_status);
		if (update_status(philo, UNCHANGED) == DEAD)
			free_exit(philo, fork_id, 0);
		check_philo_think(philo);
		sem_wait(rules->sem_fork);
		update_status(philo, TAKES_FORK);
		sem_wait(rules->sem_fork);
		if (update_status(philo, TAKES_FORK) == DEAD)
		{
			sem_post(rules->sem_fork);
			sem_post(rules->sem_fork);
			free_exit(philo, fork_id, 0);
		}
		philo_cycle(philo);
	}
}

void	serve_food(t_rules *rules, t_philo *philo, pid_t *fork_id)
{
	if (pthread_create(&rules->moni, NULL, supervise, (void *)philo) != 0)
	{
		error_msg("thread create failded\n", rules);
		free_exit(philo, fork_id, 1);
	}
	if (philo == philo->right)
	{
		update_status(philo, TAKES_FORK);
		free_exit(philo, fork_id, 0);
	}
	start_routine(rules, philo, fork_id);
}
