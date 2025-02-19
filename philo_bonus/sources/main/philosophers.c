/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:49:16 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/19 17:28:01 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	init_philo(t_philo *philo, t_rules *dining_rules, t_philo **new)
{
	static int	i;
	*new = (t_philo *)malloc(sizeof(t_philo));
	if (*new == NULL)
    {
        free_struct(philo, i);
        return (1);
    }	
	(*new)->index = (i + 1);
	(*new)->status = THINK;
	(*new)->last_meal_time = current_time();
	(*new)->meals_eaten = 0;
	(*new)->lst_rules = dining_rules;
	(*new)->right = NULL;
	(*new)->left = NULL;
	i++;
	return (0);
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

static int	create_forks_id(t_rules *dining_rules, pid_t **fork_id)
{
	int	nb;

	nb = dining_rules->nb_philo;
	*fork_id = (pid_t *)malloc(sizeof(pid_t) * nb);
	if (*fork_id == NULL)
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
	pid_t		*fork_id;

	end = NULL;
	philo = create_philo(dining_rules, &end, &new);
	if (philo == NULL)
		return (EXIT_FAILURE);
	if (create_forks_id(dining_rules, &fork_id) != EXIT_SUCCESS)
	{
		free_struct(philo, dining_rules->nb_philo);
		return (EXIT_FAILURE);
	}
	if (handle_forks(dining_rules, philo, fork_id) != EXIT_SUCCESS)
	{
		free_struct(philo, dining_rules->nb_philo);
		free(fork_id);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
