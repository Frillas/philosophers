/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:49:16 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/02 12:56:39 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static int	init_philo(t_philo *philo, t_rules *dining_rules, t_philo **new)
{
	static int	nb_philo;

	*new = (t_philo *)malloc(sizeof(t_philo));
	if (*new == NULL)
	{
		write(2, "new philosopher's memory allocation failed\n", 43);
		close_semaphores(dining_rules, 0);
		free_struct(philo, nb_philo);
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
	int		i;
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

static int	create_forks_id(t_rules *dining_rules)
{
	int	nb;

	nb = dining_rules->nb_philo;
	dining_rules->fork_id = (pid_t *)malloc(sizeof(pid_t) * nb);
	if (dining_rules->fork_id == NULL)
	{
		write(STDERR_FILENO, "Memory allocation failed to create forks\n", 41);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	start_philo(t_rules *dining_rules)
{
	t_philo		*philo;
	t_philo		*end;
	t_philo		*new;

	end = NULL;
	philo = create_philo(dining_rules, &end, &new);
	if (philo == NULL)
		return (EXIT_FAILURE);
	if (create_forks_id(dining_rules) != EXIT_SUCCESS)
	{
		close_semaphores(dining_rules, 0);
		free_struct(philo, dining_rules->nb_philo);
		return (EXIT_FAILURE);
	}
	handle_forks(dining_rules, philo);
	return (EXIT_SUCCESS);
}
