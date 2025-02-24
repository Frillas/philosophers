/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:24:13 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/24 18:42:32 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	launch(t_philo *philo, pthread_t *thread, pthread_t *moni)
{
	t_philo	*current;
	t_rules	*dining_rules;
	long	thread_created;

	thread_created = 0;
	current = philo;
	dining_rules = philo->lst_rules;
	if (pthread_create(moni, NULL, supervise, (void *)philo) != 0)
		return (EXIT_FAILURE);
	while (thread_created < dining_rules->nb_philo)
	{
		if ((pthread_create(&thread[thread_created], NULL, serve_food,
				(void *)current) != 0))
		{
			write(2, "\n\tThread create error\n\n", 23);
			err_thread(philo, thread, thread_created);
			return (EXIT_FAILURE);
		}
		current = current->right;
		thread_created++;
	}
	return (EXIT_SUCCESS);
}

static t_bool	wait_threads(t_philo *philo, pthread_t *thread)
{
	long		nb_thread_waited;
	t_philo		*current;
	t_bool		error;

	nb_thread_waited = 0;
	error = FALSE;
	current = philo;
	while (nb_thread_waited < philo->lst_rules->nb_philo)
	{
		if (pthread_join(thread[nb_thread_waited], NULL) != 0)
		{
			write(2, "\n\tthread wait error\n\n", 21);
			error = TRUE;
		}
		current = current->right;
		nb_thread_waited++;
	}
	return (error);
}

static t_bool	wait_monitor(t_philo *philo, pthread_t *monitor)
{
	t_philo	*current;
	long	nb_mutex_destroy;
	t_bool	error;

	current = philo;
	nb_mutex_destroy = 0;
	error = FALSE;
	if (pthread_join(*monitor, NULL) != 0)
	{
		write(2, "\n\tthread wait error\n\n", 21);
		error = TRUE;
	}
	if (pthread_mutex_destroy(&philo->lst_rules->status_lock) != 0)
	{
		write(2, "\n\tmutex destroy error\n\n", 23);
		error = TRUE;
	}
	return (error);
}

static t_bool	destroy_mutexes(t_philo *philo)
{
	long		nb_mutex_destroy;
	t_philo		*current;
	t_bool		error;

	nb_mutex_destroy = 0;
	error = FALSE;
	current = philo;
	while (nb_mutex_destroy < philo->lst_rules->nb_philo)
	{
		if (pthread_mutex_destroy(&current->mutex) != 0)
		{
			write(2, "\n\tmutex destroy error\n\n", 23);
			error = TRUE;
		}
		current = current->right;
		nb_mutex_destroy++;
	}
	return (error);
}

int	handle_threads(t_rules *rules, t_philo *philo, pthread_t *thread_id)
{
	pthread_t	monitor;
	int			error;

	error = 0;
	gettimeofday(&rules->start, NULL);
	if (launch(philo, thread_id, &monitor) != 0)
		error = 1;
	if (error == 0)
		wait_threads(philo, thread_id);
	if (wait_monitor(philo, &monitor) != 0)
		error = 1;
	if (destroy_mutexes(philo) != 0)
		error = 1;
	if (error)
		return (EXIT_FAILURE);
	free_struct(philo, rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
