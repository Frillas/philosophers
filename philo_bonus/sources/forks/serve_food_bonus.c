/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:54:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/27 11:26:52 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	stop(t_philo *philo, pid_t *fork_id, int res, pthread_t *moni)
{
	t_rules	*dining_rules;

	dining_rules = philo->lst_rules;
	if (res == 0)
	{
		if (pthread_join(*moni, NULL) != 0)
		{
			error_msg("thread wait failded\n", dining_rules);
			res = 1;
		}
	}
	free(fork_id);
	sem_close(dining_rules->sem_fork);
	sem_close(dining_rules->sem_status);
	free_struct(philo, dining_rules->nb_philo);
	exit(res);
}

static t_status	check_status(t_philo *philo, t_status status)
{
	sem_wait(philo->lst_rules->sem_status);
	if (philo->status == DEAD)
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

static t_status	eat_or_sleep(long duration, t_philo *philo)
{
	long	end_time;

	end_time = current_time() + duration;
	while (current_time() < end_time)
	{
		sem_wait(philo->lst_rules->sem_status);
		if (philo->status == DEAD)
		{
			sem_post(philo->lst_rules->sem_status);
			return (DEAD);
		}
		sem_post(philo->lst_rules->sem_status);
		usleep(100);
	}
	return (EXIT_SUCCESS);
}

static void	philo_set_state(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->lst_rules;
	check_status(philo, EAT);
	if (eat_or_sleep(rules->time_to_eat, philo) == DEAD)
	{
		sem_post(rules->sem_fork);
		sem_post(rules->sem_fork);
		return ;
	}
	check_status(philo, SLEEP);
	sem_post(rules->sem_fork);
	sem_post(rules->sem_fork);
	if (eat_or_sleep(rules->time_to_sleep, philo) == DEAD)
		return ;
	check_status(philo, THINK);
	sem_wait(rules->sem_status);
	if (philo->meals_eaten == rules->meals_per_philo)
		philo->status = DEAD;
	sem_post(rules->sem_status);
}

void	serve_food(t_rules *rules, t_philo *philo, pid_t *fork_id)
{
	rules->sem_fork = sem_open("/fork_sem", 0);
	rules->sem_status = sem_open("/status_sem", 0);
	rules->sem_die = sem_open("/die_sem", 0);
	if (pthread_create(&rules->moni, NULL, supervise, (void *)philo) != 0)
	{
		error_msg("thread create failded\n", rules);
		stop(philo, fork_id, 1, &rules->moni);
	}
	if (philo == philo->right)
	{
		check_status(philo, TAKES_FORK);
		stop(philo, fork_id, 0, &rules->moni);
	}
	while (1)
	{
		if (check_status(philo, UNCHANGED) == DEAD)
			stop(philo, fork_id, 0, &rules->moni);
		sem_wait(rules->sem_fork);
		check_status(philo, TAKES_FORK);
		sem_wait(rules->sem_fork);
		if (check_status(philo, TAKES_FORK) == DEAD)
		{
			sem_post(rules->sem_fork);
			sem_post(rules->sem_fork);
			stop(philo, fork_id, 0, &rules->moni);
		}
		philo_set_state(philo);
	}
}
