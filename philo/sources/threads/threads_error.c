/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:45:48 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/25 10:46:50 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void	err_wait(t_philo *philo, pthread_t *t_id, pthread_t *moni, long tot)
{
	int		thread_waited;
	t_philo	*current;

	thread_waited = 0;
	current = philo;
	while (thread_waited < tot - 1)
	{
		if (pthread_join(t_id[thread_waited], NULL) != 0)
			write(2, "thread wait error\n", 18);
		current = current->right;
		thread_waited++;
	}
	if (pthread_join(*moni, NULL) != 0)
		write(2, "thread wait error\n", 18);
}

static void	err_mutex(t_philo *philo, t_rules *dining_rules)
{
	long		mutex_destroy;
	t_philo		*current;

	mutex_destroy = 0;
	current = philo;
	while (mutex_destroy < dining_rules->nb_philo)
	{
		if (pthread_mutex_destroy(&current->mutex) != 0)
			write(2, "mutex destroy error\n", 20);
		current = current->right;
		mutex_destroy++;
	}
	if (pthread_mutex_destroy(&dining_rules->status_lock) != 0)
		write(2, "mutex destroy error\n", 20);
}

void	err_thread(t_philo *philo, pthread_t *t_id, pthread_t *moni, long tot)
{
	t_rules	*dining_rules;

	dining_rules = philo->lst_rules;
	err_wait(philo, t_id, moni, tot);
	err_mutex(philo, dining_rules);
}
