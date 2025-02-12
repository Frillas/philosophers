/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/12 19:12:35 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static int	get_time(t_rules *rules)
{
	struct timeval	end;
	int				time;

	gettimeofday(&end, NULL);
	time = (end.tv_sec - rules->start.tv_sec) * 100
		+ (end.tv_usec - rules->start.tv_usec) / 100;
	return (time);
}

static void	print_status(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->lst_rules;
	pthread_mutex_lock(&philo->lst_rules->print_lock);
	if (philo->status == EAT)
		printf("%d %d is eating\n", get_time(rules), philo->index);
	else if (philo->status == SLEEP)
		printf("%d %d is sleeping\n", get_time(rules), philo->index);
	else if (philo->status == THINK)
		printf("%d %d is thinking\n", get_time(rules), philo->index);
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

static void	*serve_food(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->index % 2 == 0)
		{
			if (pthread_mutex_lock(&philo->mutex) == 0)
			{
				pthread_mutex_lock(&philo->lst_rules->print_lock);
				printf("%d %d has taken a fork\n", get_time(philo->lst_rules), philo->index);
				pthread_mutex_unlock(&philo->lst_rules->print_lock);
			}
			if (pthread_mutex_lock(&philo->left->mutex) == 0)
			{
				pthread_mutex_lock(&philo->lst_rules->print_lock);
				printf("%d %d has taken a fork\n", get_time(philo->lst_rules), philo->index);
				pthread_mutex_unlock(&philo->lst_rules->print_lock);
			}
		}
		else
		{
			if (pthread_mutex_lock(&philo->left->mutex) == 0)
			{
				pthread_mutex_lock(&philo->lst_rules->print_lock);
				printf("%d %d has taken a fork\n", get_time(philo->lst_rules), philo->index);
				pthread_mutex_unlock(&philo->lst_rules->print_lock);
			}
			if (pthread_mutex_lock(&philo->mutex) == 0)
			{
				pthread_mutex_lock(&philo->lst_rules->print_lock);
				printf("%d %d has taken a fork\n", get_time(philo->lst_rules), philo->index);
				pthread_mutex_unlock(&philo->lst_rules->print_lock);
			}
		}
		update_status(philo);
		break ;
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
