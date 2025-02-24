/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:24:13 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/24 16:37:14 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	launch_threads(t_philo *philo, pthread_t *thread, pthread_t *moni)
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
				(void *)current) != 0) || thread_created == 4)
		{
			err_thread(philo, moni, thread, thread_created + 1);
			return (EXIT_FAILURE);
		}
		current = current->right;
		thread_created++;
	}
	return (EXIT_SUCCESS);
}

static t_bool	wait_threads(t_philo *philo, pthread_t *thread, pthread_t *monitor)
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
			write(2, "thread wait error\n", 18);
			error = TRUE;
		}
		current = current->right;
		nb_thread_waited++;
	}
	if (pthread_join(*monitor, NULL) != 0)
	{
		write(2, "thread wait error\n", 18);
		error = TRUE;
	}
	return (error);
}

static t_bool	destroy_mutexes(t_philo *philo, t_rules *dining_rules)
{
	t_philo	*current;
	long	nb_mutex_destroy;
	t_bool	error;

	current = philo;
	nb_mutex_destroy = 0;
	error = FALSE;
	while (nb_mutex_destroy < dining_rules->nb_philo)
	{
		if (pthread_mutex_destroy(&current->mutex) != 0)
		{
			write(2, "thread destroy error\n", 21);
			error = TRUE;
		}
		current = current->right;
		nb_mutex_destroy++;
	}
	if (pthread_mutex_destroy(&dining_rules->status_lock) != 0)
	{
		write(2, "thread destroy error\n", 21);
		error = TRUE;
	}
	return (error);
}

int	handle_threads(t_rules *rules, t_philo *philo, pthread_t *thread_id)
{
	pthread_t	monitor;

	gettimeofday(&rules->start, NULL);
	if (launch_threads(philo, thread_id, &monitor) != 0)
		return (EXIT_FAILURE);
	if (wait_threads(philo, thread_id, &monitor) != 0)
		return (EXIT_FAILURE);
	if (destroy_mutexes(philo, rules) != 0)
		return (EXIT_FAILURE);
	free_struct(philo, rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
