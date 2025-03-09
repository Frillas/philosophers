/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:24:13 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/09 11:48:33 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	launch(t_philo *philo, pthread_t *thread, pthread_t *moni)
{
	t_philo	*current;
	long	thread_created;

	thread_created = 0;
	current = philo;
	if (pthread_create(moni, NULL, supervise, (void *)philo) != 0)
	{
		write(2, "Thread create error\n", 20);
		err_thread(philo, thread, moni, 0);
		return (EXIT_FAILURE);
	}
	while (thread_created < philo->lst_rules->nb_philo)
	{
		if (pthread_create(&thread[thread_created], NULL, serve_food,
				(void *)current) != 0)
		{
			write(2, "Thread create error\n", 20);
			err_thread(philo, thread, moni, thread_created + 1);
			return (EXIT_FAILURE);
		}
		current = current->right;
		thread_created++;
	}
	return (EXIT_SUCCESS);
}

static t_bool	wait_threads(t_philo *philo, pthread_t *thread, pthread_t *moni)
{
	long		thread_waited;
	t_philo		*current;
	t_bool		error;

	thread_waited = 0;
	error = FALSE;
	current = philo;
	while (thread_waited < philo->lst_rules->nb_philo)
	{
		if (pthread_join(thread[thread_waited], NULL) != 0)
		{
			write(2, "thread wait error\n", 18);
			error = TRUE;
		}
		current = current->right;
		thread_waited++;
	}
	if (pthread_join(*moni, NULL) != 0)
	{
		write(2, "thread wait error\n", 18);
		error = TRUE;
	}
	return (error);
}

static t_bool	destroy_mutexes(t_philo *philo, t_rules *dining_rules)
{
	long		mutex_destroy;
	t_philo		*current;
	t_bool		error;

	mutex_destroy = 0;
	error = FALSE;
	current = philo;
	while (mutex_destroy < dining_rules->nb_philo)
	{
		if (destroy_mutex(&current->fork_mutex) != 0)
		{
			write(2, "mutex destroy error\n", 20);
			error = TRUE;
		}
		current = current->right;
		mutex_destroy++;
	}
	if (destroy_mutex(&dining_rules->status_lock) == TRUE)
		error = TRUE;
	if (destroy_mutex(&dining_rules->meals_lock) == TRUE)
		error = TRUE;
	return (error);
}

static void	init_time_to_think(t_rules *rules)
{
	rules->time_to_think = (rules->time_to_eat + rules->time_to_sleep) / 2;
	rules->is_hungry = rules->time_to_die % 100;
	rules->is_hungry = (rules->time_to_die - 100) - rules->is_hungry;
	if (rules->time_to_think < 100)
		rules->is_hungry = 180;
}

int	handle_threads(t_rules *rules, t_philo *philo, pthread_t *thread_id)
{
	pthread_t	monitor;
	t_bool		error;

	error = 0;
	init_time_to_think(rules);
	gettimeofday(&rules->start, NULL);
	if (launch(philo, thread_id, &monitor) != 0)
		return (EXIT_FAILURE);
	if (wait_threads(philo, thread_id, &monitor) != 0)
		error = TRUE;
	if (destroy_mutexes(philo, rules) != 0)
		error = TRUE;
	if (error == TRUE)
		return (EXIT_FAILURE);
	free_struct(philo, rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
