/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:49:16 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/25 11:57:01 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	init_philo(t_philo *philo, t_rules *dining_rules, t_philo **new)
{
	static long	nb_philo;

	*new = (t_philo *)malloc(sizeof(t_philo));
	if (*new == NULL)
	{
		write(2, "new philosopher's memory allocation failed\n", 43);
		err_init_philo(philo, nb_philo);
		return (EXIT_FAILURE);
	}
	if (pthread_mutex_init(&(*new)->mutex, NULL) != 0)
	{
		write(2, "mutex init error\n", 17);
		free(*new);
		err_init_philo(philo, nb_philo);
		return (EXIT_FAILURE);
	}
	(*new)->index = (nb_philo + 1);
	(*new)->status = THINK;
	(*new)->last_meal_time = current_time();
	(*new)->meals_eaten = 0;
	(*new)->lst_rules = dining_rules;
	(*new)->right = NULL;
	(*new)->left = NULL;
	nb_philo++;
	return (EXIT_SUCCESS);
}

static t_philo	*create_philo(t_rules *rules, t_philo **end, t_philo **new)
{
	long	i;
	t_philo	*philo;

	i = 0;
	philo = NULL;
	while (i < rules->nb_philo)
	{
		if (init_philo(philo, rules, new) != 0)
			return (NULL);
		else if (philo == NULL)
		{
			philo = *new;
			*end = *new;
		}
		else
		{
			(*end)->right = *new;
			(*new)->left = *end;
			(*end) = *new;
		}
		i++;
	}
	philo->left = *end;
	(*end)->right = philo;
	return (philo);
}

static int	create_thread_id(t_rules *dining_rules, pthread_t **tid)
{
	long	nb;

	nb = dining_rules->nb_philo;
	*tid = (pthread_t *)malloc(sizeof(pthread_t) * nb);
	if (*tid == NULL)
	{
		write(STDERR_FILENO, "Memory allocation failed for threads\n", 37);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_philo(t_rules *dining_rules)
{
	t_philo		*philo;
	t_philo		*end;
	t_philo		*new;
	pthread_t	*thread_id;

	end = NULL;
	philo = create_philo(dining_rules, &end, &new);
	if (philo == NULL)
		return (EXIT_FAILURE);
	if (create_thread_id(dining_rules, &thread_id) != EXIT_SUCCESS)
	{
		err_init_philo(philo, dining_rules->nb_philo);
		return (EXIT_FAILURE);
	}
	if (handle_threads(dining_rules, philo, thread_id) != EXIT_SUCCESS)
	{
		free_struct(philo, dining_rules->nb_philo);
		free(thread_id);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
