/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/10 14:09:01 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void test(t_philo *philo)
{
	printf("index = %d\n", philo->index);
	printf("le philo mange %d\n", philo->status);
	philo->status = EAT;
	printf("le philo a mange = %d\n", philo->status);
}

static void	*eating(void *arg)
{
	t_philo	*philo;
	
	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->mutex);
	test(philo);
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

int	start_dinner(t_rules *dinning_rules, t_philo *philo, pthread_t *thread_id)
{
	int		i;
	t_philo	*current;
	
	i = 0;
	(void)dinning_rules;
	current = philo;
	while (i < dinning_rules->nb_philo)
	{
		pthread_mutex_init(&current->mutex, NULL);
		if (pthread_create(&thread_id[i], NULL, eating, (void *)current) != 0)
			return (EXIT_FAILURE);
		current = current->right;
		i++;
	}
	i = 0;
	current = philo;
	while (i < dinning_rules->nb_philo)
	{
		if (pthread_join(thread_id[i], NULL) != 0)
			return (EXIT_FAILURE);
		pthread_mutex_destroy(&current->mutex);
		current = current->right;
		i++;
	}
	free_struct(philo, dinning_rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
