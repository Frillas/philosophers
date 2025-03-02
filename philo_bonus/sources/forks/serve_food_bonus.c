/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:54:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/02 12:58:23 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	free_exit(t_philo *philo, pid_t *fork_id, int err_thread)
{
	t_rules	*dining_rules;

	dining_rules = philo->lst_rules;
	if (err_thread == 0)
	{
		if (pthread_join(dining_rules->moni, NULL) != 0)
		{
			error_msg("thread wait failded\n", dining_rules);
			err_thread = 1;
		}
	}
	close_semaphores(dining_rules, 1);
	free(fork_id);
	free_struct(philo, dining_rules->nb_philo);
	exit(err_thread);
}

static t_status	update_status(t_philo *philo, t_status status)
{
	sem_wait(philo->lst_rules->sem_status);
	if (philo->status == DEAD || philo->status == SATIATED)
	{
		sem_post(philo->lst_rules->sem_status);
		return (DEAD);
	}
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		if (philo->status == EAT)
		{
			philo->last_meal_time = current_time();
			philo->meals_eaten++;
		}
	}
	sem_post(philo->lst_rules->sem_status);
	return (EXIT_SUCCESS);
}

static t_status	wait_with_death_check(time_t duration, t_philo *philo)
{
	time_t		end_time;
	t_status	status;

	end_time = current_time() + duration;
	while (current_time() < end_time)
	{
		sem_wait(philo->lst_rules->sem_status);
		status = philo->status;
		sem_post(philo->lst_rules->sem_status);
		if (status == DEAD)
			return (DEAD);
		usleep(100);
	}
	return (EXIT_SUCCESS);
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
	update_status(philo, SLEEP);
	sem_post(rules->sem_fork);
	sem_post(rules->sem_fork);
	if (wait_with_death_check(rules->time_to_sleep, philo) == DEAD)
		return ;
	update_status(philo, THINK);
	sem_wait(rules->sem_status);
	if (philo->meals_eaten == rules->meals_per_philo)
	{
		philo->status = SATIATED;
		sem_post(rules->sem_eat);
	}
	sem_post(rules->sem_status);
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
	while (1)
	{
		if (update_status(philo, UNCHANGED) == DEAD)
			free_exit(philo, fork_id, 0);
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
