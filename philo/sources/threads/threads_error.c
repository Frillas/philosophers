/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:45:48 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/24 20:56:37 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void	error(t_philo *philo, pthread_t *thread, pthread_t *moni, long tot)
{
	int		nb_thread_waited;
	t_philo	*current;

	nb_thread_waited = 0;
	current = philo;
	while (nb_thread_waited < tot - 1)
	{
		if (pthread_join(thread[nb_thread_waited], NULL) != 0)
			write(2, "thread wait error\n", 18);
		current = current->right;
		nb_thread_waited++;
	}
	if (pthread_join(*moni, NULL) != 0)
		write(2, "\n\tthread wait error\n\n", 21);
}

static void	err_destroy(t_philo *philo, t_rules *dining_rules)
{
	long		nb_mutex_destroy;
	t_philo		*current;

	nb_mutex_destroy = 0;
	current = philo;
	while (nb_mutex_destroy < dining_rules->nb_philo)
	{
		if (pthread_mutex_destroy(&current->mutex) != 0)
			write(2, "\n\tmutex destroy error\n\n", 23);
		current = current->right;
		nb_mutex_destroy++;
	}
	if (pthread_mutex_destroy(&dining_rules->status_lock) != 0)
		write(2, "\n\tmutex destroy error\n\n", 23);
}

void	err_thread(t_philo *philo, pthread_t *t_id, pthread_t *moni, long tot)
{
	t_rules	*dining_rules;

	if (tot == 0)
		return ;
	dining_rules = philo->lst_rules;
	error(philo, t_id, moni, tot);
	err_destroy(philo, dining_rules);
}
