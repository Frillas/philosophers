/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:45:48 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/24 16:48:55 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void	err_wait(t_philo *philo, pthread_t *moni, pthread_t *thread, int created)
{
	int		nb_thread_waited;
	t_philo	*current;

	nb_thread_waited = 0;
	current = philo;
	if (created)
	{	
		if (pthread_join(*moni, NULL) != 0)
			write(2, "thread wait error\n", 18);
		created--;
	}
	while (nb_thread_waited < created)
	{
		if (pthread_join(thread[nb_thread_waited], NULL) != 0)
			write(2, "thread wait error\n", 18);
		current = current->right;
		nb_thread_waited++;
	}
}

static void err_destroy(t_philo *philo, long created)
{
	t_rules	*dining_rules;
	t_philo	*current;
	long	nb_destroy_mutex;

	dining_rules = philo->lst_rules;
	current = philo;
	nb_destroy_mutex = 0;
	while (nb_destroy_mutex < created)
	{
		if (pthread_mutex_destroy(&current->mutex) != 0)
			write(2, "mutex destroy error\n", 20);
		current = current->right;
		nb_destroy_mutex++;
	}
	if (pthread_mutex_destroy(&dining_rules->status_lock) != 0)
		write(2, "mutex destroy error\n", 20);
}

void err_thread(t_philo *philo, pthread_t *moni, pthread_t *thread, long created)
{
	err_wait(philo, moni, thread, created);
	err_destroy(philo, created - 1);
}
