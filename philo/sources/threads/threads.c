/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:24:13 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/25 14:25:02 by aroullea         ###   ########.fr       */
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
		if (pthread_mutex_destroy(&current->mutex) != 0)
		{
			write(2, "mutex destroy error\n", 20);
			error = TRUE;
		}
		current = current->right;
		mutex_destroy++;
	}
	if (pthread_mutex_destroy(&dining_rules->status_lock) != 0)
	{
		write(2, "mutex destroy error\n", 20);
		error = TRUE;
	}
	return (error);
}

int	handle_threads(t_rules *rules, t_philo *philo, pthread_t *thread_id)
{
	pthread_t	monitor;
	t_bool		error;

	error = 0;
	if (gettimeofday(&rules->start, NULL) != 0)
	{
		write(2, "Get time of day error\n", 22);
		err_thread(philo, thread_id, &monitor, 0);
		return (EXIT_FAILURE);
	}
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
