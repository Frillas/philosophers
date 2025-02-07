/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/07 12:04:18 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

pthread_mutex_t mutex;

static void	*eating(void *arg)
{
	t_philo	*philo;
	
	philo = (t_philo *)arg;
	if (philo->left->status == EAT)
	pthread_mutex_lock(&mutex);
	philo->status = EAT;
	philo->r_fork = 0;
	printf("philo is eating\n");
	philo->status = SLEEP;
	printf("philo is sleeping\n");
	pthread_mutex_unlock(&mutex);
	return (NULL);
}

int	start_dinner(t_rules *dinning_rules, t_philo *philo, pthread_t *thread_id)
{
	int		i;
	t_philo	*current;

	i = 0;
	(void)dinning_rules;
	current = philo;
	pthread_mutex_init(&mutex, NULL);
	while (i < dinning_rules->nb_philo)
	{
		if (pthread_create(&thread_id[i], NULL, eating, philo) != 0)
			return (EXIT_FAILURE);
		current = current->right;
		i++;
	}
	i = 0;
	while (i < dinning_rules->nb_philo)
	{
		if (pthread_join(thread_id[i], NULL) != 0)
			return (EXIT_FAILURE);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	free_struct(philo, dinning_rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
