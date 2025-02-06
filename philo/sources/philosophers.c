/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:49:16 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/06 17:57:46 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static t_philo	*initialize_philo(t_philo *philo)
{
	t_philo	*new;

	new = (t_philo *)malloc(sizeof(t_philo));
	if (new == NULL)
	{
		free_struct(philo);
		return (NULL);
	}	
	new->fork = 1;
	new->status = 1;
	new->right = NULL;
	new->left = NULL;
	return (new);
}

static t_philo	*create_philo(t_rules *dinning_rules, t_philo **end)
{
	int		i;
	t_philo	*philo;
	t_philo *new;

	i = 0;
	philo = NULL;
	while (i < dinning_rules->nb_philo)
	{
		new = initialize_philo(philo);
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
	return (philo);
}

void	start_philo(t_rules *dinning_rules)
{
	t_philo	*philo;
	t_philo	*end;

	end = NULL;
	philo = create_philo(dinning_rules, &end);
	if (philo == NULL)
		return ;
	start_dinner(dinning_rules, philo);
}
