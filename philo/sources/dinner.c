/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/13 14:37:44 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	print_status(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->lst_rules;
	pthread_mutex_lock(&philo->lst_rules->print_lock);
	if (philo->status == EAT)
		printf("%ld %d is eating\n", step_timer(rules), philo->index);
	else if (philo->status == SLEEP)
		printf("%ld %d is sleeping\n", step_timer(rules), philo->index);
	else if (philo->status == THINK)
		printf("%ld %d is thinking\n", step_timer(rules), philo->index);
	pthread_mutex_unlock(&philo->lst_rules->print_lock);
}

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
	t_philo	*philo;
	t_rules	*rules;
	pthread_mutex_t	*first_mutex;
	pthread_mutex_t	*second_mutex;

	philo = (t_philo *)arg;
	rules = philo->lst_rules;
	first_mutex = &philo->mutex;
	second_mutex = &philo->left->mutex;
	while (1)
	{
		if (current_time() - philo->last_meal_time > rules->time_to_die)
		{
			printf("%ld %d died\n", step_timer(rules), philo->index);
			return (NULL);
		}
		if (philo->index > philo->left->index)
		{
			first_mutex = &philo->left->mutex;
			second_mutex = &philo->mutex;
		}
		pthread_mutex_lock(first_mutex);
		pthread_mutex_lock(second_mutex);
		update_status(philo);
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
