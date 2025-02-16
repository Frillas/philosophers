/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/16 11:36:15 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	eat_or_sleep(long duration)
{
	long	runtime;

	runtime = 0;
	while (runtime < duration)
	{
		usleep(10000);
		runtime += 10000;
	}
}

static int	check_status(t_philo *philo, t_status status)
{
	pthread_mutex_lock(&philo->lst_rules->status_lock);
	if (philo->status == DEAD)
	{
		pthread_mutex_unlock(&philo->lst_rules->status_lock);
		return (1);
	}
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		pthread_mutex_unlock(&philo->lst_rules->status_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->lst_rules->status_lock);
	return (0);
}

static void	philo_set_state(t_philo *philo)
{
	check_status(philo, EAT);
	eat_or_sleep(philo->lst_rules->time_to_eat);
	philo->last_meal_time = current_time();
	check_status(philo, SLEEP);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(&philo->left->mutex);
	eat_or_sleep(philo->lst_rules->time_to_sleep);
	check_status(philo, THINK);
}

static void	swap(t_philo *philo, pthread_mutex_t **one, pthread_mutex_t **two)
{
	*one = &philo->mutex;
	*two = &philo->left->mutex;
	if (philo->index > philo->left->index)
	{
		*one = &philo->left->mutex;
		*two = &philo->mutex;
	}
}

static void	*serve_food(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first_mutex;
	pthread_mutex_t	*second_mutex;

	philo = (t_philo *)arg;
	swap(philo, &first_mutex, &second_mutex);
	while (1)
	{
		if (check_status(philo, UNCHANGED) == 1)
			return (NULL);
		pthread_mutex_lock(first_mutex);
		check_status(philo, TAKES_FORK);
		pthread_mutex_lock(second_mutex);
		if (check_status(philo, TAKES_FORK) == 1)
		{
			pthread_mutex_unlock(first_mutex);
			pthread_mutex_unlock(second_mutex);
			return (NULL);
		}
		philo_set_state(philo);
	}
	return (NULL);
}

int	start_dinner(t_rules *dining_rules, t_philo *philo, pthread_t *thread_id)
{
	int				i;
	t_philo			*current;
	pthread_t		monitor;

	i = 0;
	(void)dining_rules;
	current = philo;
	gettimeofday(&dining_rules->start, NULL);
	if (pthread_create(&monitor, NULL, supervise, (void *)philo) != 0)
		return (EXIT_FAILURE);
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
	pthread_join(monitor, NULL);
	i = 0;
	current = philo;
	while (i < dining_rules->nb_philo)
	{
		pthread_mutex_destroy(&current->mutex);
		current = current->right;
		i++;
	}
	pthread_mutex_destroy(&dining_rules->status_lock);
	free_struct(philo, dining_rules->nb_philo);
	free(thread_id);
	return (EXIT_SUCCESS);
}
