/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/12 13:51:50 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	print_status(t_philo *philo)
{
	pthread_mutex_lock(&philo->lst_rules->print_lock);
	if (philo->status == EAT)
		printf("%d is eating\n", philo->index);
	else if (philo->status == SLEEP)
		printf("%d is sleeping\n", philo->index);
	else if (philo->status == THINK)
		printf("%d is thinking\n", philo->index);
	pthread_mutex_unlock(&philo->lst_rules->print_lock);
}

static void	update_status(t_philo *philo)
{
	philo->status = EAT;
	print_status(philo);
	usleep(philo->lst_rules->time_to_eat);
	philo->status = SLEEP;
	print_status(philo);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(&philo->left->mutex);
	usleep(philo->lst_rules->time_to_sleep);
	philo->status = THINK;
	print_status(philo);
}

static void	*serve(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->index % 2 == 0)
		{
			pthread_mutex_lock(&philo->mutex);
			pthread_mutex_lock(&philo->left->mutex);
		}
		else
		{
			pthread_mutex_lock(&philo->left->mutex);
			pthread_mutex_lock(&philo->mutex);
		}
		update_status(philo);
		break;
	}
	return (NULL);
}

int	start_dinner(t_rules *dinning_rules, t_philo *philo, pthread_t *thread_id)
{
	int				i;
	t_philo			*current;

	i = 0;
	(void)dinning_rules;
	current = philo;
	while (i < dinning_rules->nb_philo)
	{
		if (pthread_create(&thread_id[i], NULL, serve, (void *)current) != 0)
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
		current = current->right;
		i++;
	}
	i = 0;
	current = philo;
	while (i < dinning_rules->nb_philo)
	{
		pthread_mutex_destroy(&current->mutex);
		current = current->right;
		i++;
	}
	pthread_mutex_destroy(&dinning_rules->print_lock);
	free_struct(philo, dinning_rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
