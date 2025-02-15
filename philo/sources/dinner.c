/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/15 12:25:40 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	eat_or_sleep(t_philo *philo, long duration, int i)
{
	t_philo	*current;
	long	runtime;

	current = philo;
	runtime = 0;
	while (runtime < duration)
	{
		usleep(10000);
		while (i < philo->lst_rules->nb_philo)
		{
			pthread_mutex_lock(&philo->lst_rules->status_lock);
			if (current->status != EAT)
			{
				pthread_mutex_unlock(&philo->lst_rules->status_lock);
				check_last_meal(current, philo->lst_rules);
			}
			else
				pthread_mutex_unlock(&philo->lst_rules->status_lock);
			current = current->right;
			i++;
		}
		i = 0;
		runtime += 10000;
	}
}

static void	update_status(t_philo *philo)
{
	pthread_mutex_lock(&philo->lst_rules->status_lock);
	philo->status = EAT;
	print_status(philo);
	pthread_mutex_unlock(&philo->lst_rules->status_lock);
	eat_or_sleep(philo, philo->lst_rules->time_to_eat, 0);
	philo->last_meal_time = current_time();
	pthread_mutex_lock(&philo->lst_rules->status_lock);
	philo->status = SLEEP;
	print_status(philo);
	pthread_mutex_unlock(&philo->lst_rules->status_lock);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(&philo->left->mutex);
	eat_or_sleep(philo, philo->lst_rules->time_to_sleep, 0);
	if (philo->status != DEAD)
	{
		pthread_mutex_lock(&philo->lst_rules->status_lock);
		philo->status = THINK;
		print_status(philo);
		pthread_mutex_unlock(&philo->lst_rules->status_lock);
	}
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
	if (philo->index > philo->left->index)
	{
		first_mutex = &philo->left->mutex;
		second_mutex = &philo->mutex;
	}
	while (1)
	{
		pthread_mutex_lock(&philo->lst_rules->status_lock);
		if (philo->status == DEAD)
		{
			pthread_mutex_unlock(&philo->lst_rules->status_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->lst_rules->status_lock);
		pthread_mutex_lock(first_mutex);
		check_last_meal(philo, rules);
		pthread_mutex_lock(&rules->status_lock);
		if (philo->status != DEAD)
		{
			philo->status = TAKES_FORK;
			print_status(philo);
		}
		pthread_mutex_unlock(&rules->status_lock);
		pthread_mutex_lock(second_mutex);
		check_last_meal(philo, rules);
		pthread_mutex_lock(&rules->status_lock);
		if (philo->status == DEAD)
		{
			pthread_mutex_unlock(first_mutex);
			pthread_mutex_unlock(second_mutex);
			pthread_mutex_unlock(&rules->status_lock);
			return (NULL);
		}
		else
		{
			philo->status = TAKES_FORK;
			print_status(philo);
			pthread_mutex_unlock(&rules->status_lock);
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
