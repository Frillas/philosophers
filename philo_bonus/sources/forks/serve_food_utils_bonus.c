/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serve_food_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 11:24:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/04 16:40:21 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

void	free_exit(t_philo *philo, pid_t *fork_id, int err_thread)
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

t_status	wait_with_death_check(time_t duration, t_philo *philo)
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

t_status	update_status(t_philo *philo, t_status status)
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

void	philo_wait(t_philo *philo)
{
	time_t	time_to_wait;
	t_rules	*rules;

	rules = philo->lst_rules;
	time_to_wait = (rules->time_to_eat + rules->time_to_sleep) / 2;
	time_to_wait += current_time();
	while (current_time() < time_to_wait)
		usleep(300);
}
