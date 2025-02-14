/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/14 12:44:14 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	update_status(t_philo *philo)
{
	philo->status = EAT;
	print_status(philo);
	usleep(philo->lst_rules->time_to_eat);
	philo->last_meal_time = current_time();
	philo->status = SLEEP;
	print_status(philo);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(&philo->left->mutex);
	usleep(philo->lst_rules->time_to_sleep);
	philo->status = THINK;
	print_status(philo);
}

static void	*serve_food(void *arg)
{
	t_philo			*philo;
	t_rules			*rules;
	pthread_mutex_t	*first_mutex;
	pthread_mutex_t	*second_mutex;

	philo = (t_philo *)arg;
	rules = philo->lst_rules;
	first_mutex = &philo->mutex;
	second_mutex = &philo->left->mutex;
	while (1)
	{
		if (philo->status == DEAD || check_last_meal(philo, rules) != 0)
			return (NULL);
		if (philo->index > philo->left->index)
		{
			first_mutex = &philo->left->mutex;
			second_mutex = &philo->mutex;
		}
		pthread_mutex_lock(first_mutex);
		check_last_meal(philo, rules);
		if (philo->status != DEAD)
		{
			philo->status = TAKES_FORK;
			print_status(philo);
		}
		pthread_mutex_lock(second_mutex);
		if (philo->status == DEAD || (check_last_meal(philo, rules) != 0))
		{
			pthread_mutex_unlock(first_mutex);
			pthread_mutex_unlock(second_mutex);
			return (NULL);
		}
		else
		{
			philo->status = TAKES_FORK;
			print_status(philo);
			update_status(philo);
		}
	}
	return (NULL);
}

int	start_dinner(t_rules *dining_rules, t_philo *philo, pthread_t *thread_id)
{
	int				i;
	t_philo			*current;

	i = 0;
	(void)dining_rules;
	current = philo;
	gettimeofday(&dining_rules->start, NULL);
	while (i < dining_rules->nb_philo)
	{
		if (pthread_create(&thread_id[i], NULL, serve_food,
				(void *)current) != 0)
			return (EXIT_FAILURE);
		current = current->right;
		i++;
	}
	i = 0;
	current = philo;
	while (i < dining_rules->nb_philo)
	{
		if (pthread_join(thread_id[i], NULL) != 0)
			return (EXIT_FAILURE);
		current = current->right;
		i++;
	}
	i = 0;
	current = philo;
	while (i < dining_rules->nb_philo)
	{
		pthread_mutex_destroy(&current->mutex);
		current = current->right;
		i++;
	}
	pthread_mutex_destroy(&dining_rules->print_lock);
	free_struct(philo, dining_rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
