/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:48:59 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/16 16:15:58 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	launch_threads(t_philo *philo, pthread_t *thread, pthread_t *moni)
{
	t_philo	*current;
	t_rules	*dining_rules;
	int		i;

	i = 0;
	current = philo;
	dining_rules = philo->lst_rules;
	if (pthread_create(moni, NULL, supervise, (void *)philo) != 0)
		return (EXIT_FAILURE);
	while (i < dining_rules->nb_philo)
	{
		if (pthread_create(&thread[i], NULL, serve_food,
				(void *)current) != 0)
			return (EXIT_FAILURE);
		current = current->right;
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	wait_threads(t_philo *philo, pthread_t *thread, pthread_t *monitor)
{
	t_philo	*current;
	t_rules	*dining_rules;
	int		i;

	i = 0;
	current = philo;
	dining_rules = philo->lst_rules;
	while (i < dining_rules->nb_philo)
	{
		if (pthread_join(thread[i], NULL) != 0)
			return (EXIT_FAILURE);
		current = current->right;
		i++;
	}
	if (pthread_join(*monitor, NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	destroy_mutexes(t_philo *philo, t_rules *dining_rules)
{
	t_philo	*current;
	int		i;

	i = 0;
	current = philo;
	while (i < dining_rules->nb_philo)
	{
		if (pthread_mutex_destroy(&current->mutex) != 0)
			return (EXIT_FAILURE);
		current = current->right;
		i++;
	}
	if (pthread_mutex_destroy(&dining_rules->status_lock) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	handle_threads(t_rules *dining_rules, t_philo *philo, pthread_t *thread_id)
{
	pthread_t		monitor;

	(void)dining_rules;
	gettimeofday(&dining_rules->start, NULL);
	if (launch_threads(philo, thread_id, &monitor) != 0)
		return (EXIT_FAILURE);
	if (wait_threads(philo, thread_id, &monitor) != 0)
		return (EXIT_FAILURE);
	if (destroy_mutexes(philo, dining_rules) != 0)
		return (EXIT_FAILURE);
	free_struct(philo, dining_rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
