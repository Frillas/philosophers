/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:49:16 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/12 17:32:13 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static t_philo	*initialize_philo(t_philo *philo, t_rules *dining_rules)
{
	t_philo		*new;
	static int	i;

	new = (t_philo *)malloc(sizeof(t_philo));
	if (new == NULL)
	{
		free_struct(philo, dining_rules->nb_philo);
		return (NULL);
	}
	if (pthread_mutex_init(&new->mutex, NULL) != 0)
		return (NULL);
	new->index = (i + 1);
	new->status = 1;
	new->lst_rules = dining_rules;
	new->right = NULL;
	new->left = NULL;
	i++;
	return (new);
}

static t_philo	*create_philo(t_rules *dining_rules, t_philo **end)
{
	int		i;
	t_philo	*philo;
	t_philo	*new;

	i = 0;
	philo = NULL;
	while (i < dining_rules->nb_philo)
	{
		new = initialize_philo(philo, dining_rules);
		if (new == NULL)
			return (NULL);
		else if (philo == NULL)
		{
			philo = new;
			*end = new;
		}
		else
		{
			(*end)->right = new;
			new->left = (*end);
			(*end) = new;
		}
		i++;
	}
	philo->left = (*end);
	(*end)->right = philo;
	return (philo);
}

static int	create_thread_id(t_rules *dining_rules, pthread_t **tid)
{
	int	nb;

	nb = dining_rules->nb_philo;
	*tid = (pthread_t *)malloc(sizeof(pthread_t) * nb);
	if (*tid == NULL)
	{
		write(STDERR_FILENO, "Memory allocation failed for threads\n", 37);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	start_philo(t_rules *dining_rules)
{
	t_philo		*philo;
	t_philo		*end;
	pthread_t	*thread_id;

	end = NULL;
	philo = create_philo(dining_rules, &end);
	if (philo == NULL)
		return (EXIT_FAILURE);
	if (create_thread_id(dining_rules, &thread_id) != EXIT_SUCCESS)
	{
		free_struct(philo, dining_rules->nb_philo);
		return (EXIT_FAILURE);
	}
	if (start_dinner(dining_rules, philo, thread_id) != EXIT_SUCCESS)
	{
		free_struct(philo, dining_rules->nb_philo);
		free(thread_id);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
