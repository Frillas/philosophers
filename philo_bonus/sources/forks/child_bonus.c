/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:54:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/22 06:04:55 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	stop(t_philo *philo, pid_t *fork_id, int res, pthread_t *moni)
{
	t_rules	*dining_rules;

	if (pthread_join(*moni, NULL) != 0)
		write(2, "thread wait failded\n", 20);
	dining_rules = philo->lst_rules;
	free(fork_id);
	sem_close(dining_rules->sem_fork);
	sem_close(dining_rules->sem_status);
	free_struct(philo, dining_rules->nb_philo);
	exit(res);
}

static int	check_status(t_philo *philo, t_status status)
{
	sem_wait(philo->lst_rules->sem_status);
	if (philo->status == DEAD)
	{
		sem_post(philo->lst_rules->sem_status);
		return (1);
	}
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		if (philo->status == EAT)
			philo->meals_eaten++;
		sem_post(philo->lst_rules->sem_status);
		return (0);
	}
	sem_post(philo->lst_rules->sem_status);
	return (0);
}

static void	eat_or_sleep(long duration, t_philo *philo)
{
	long	runtime;

	runtime = 0;
	duration *= 1000;
	while (runtime < duration)
	{
		sem_wait(philo->lst_rules->sem_status);
		if (philo->status == DEAD)
		{
			sem_post(philo->lst_rules->sem_status);
			return ;
		}
		sem_post(philo->lst_rules->sem_status);
		usleep(10000);
		runtime += 10000;
	}
}

static void	philo_set_state(t_philo *philo)
{
	check_status(philo, EAT);
	eat_or_sleep(philo->lst_rules->time_to_eat, philo);
	philo->last_meal_time = current_time();
	check_status(philo, SLEEP);
	sem_post(philo->lst_rules->sem_fork);
	sem_post(philo->lst_rules->sem_fork);
	eat_or_sleep(philo->lst_rules->time_to_sleep, philo);
	check_status(philo, THINK);
	sem_wait(philo->lst_rules->sem_status);
	if (philo->meals_eaten == philo->lst_rules->meals_per_philo)
		philo->status = DEAD;
	sem_post(philo->lst_rules->sem_status);
}

void	serve_food(t_rules *dining_rules, t_philo *philo, pid_t *fork_id)
{
	pthread_t	moni;

	if (pthread_create(&moni, NULL, supervise, (void *)philo) != 0)
		stop(philo, fork_id, 1, &moni);
	if (philo == philo->right)
	{
		check_status(philo, TAKES_FORK);
		stop(philo, fork_id, 0, &moni);
	}
	while (1)
	{
		if (check_status(philo, UNCHANGED) == 1)
			stop(philo, fork_id, 0, &moni);
		sem_wait(dining_rules->sem_fork);
		check_status(philo, TAKES_FORK);
		sem_wait(dining_rules->sem_fork);
		if (check_status(philo, TAKES_FORK) == 1)
		{
			sem_post(dining_rules->sem_fork);
			sem_post(dining_rules->sem_fork);
			stop(philo, fork_id, 0, &moni);
		}
		philo_set_state(philo);
	}
}
