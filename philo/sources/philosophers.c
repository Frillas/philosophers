/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:49:16 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/06 11:48:00 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static t_philo	create_philo(t_philo *philo, t_rules *dinning_rules)
{
	t_philo	*new;

	new = (t_philo *)malloc(sizeof(t_philo));
	if (new == NULL)
	{
		free_struct(philo);
		return (NULL);
	}	
	new->fork = 1;
	new->status = 0;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	start_philo(t_rules *dinning_rules)
{
	int		i;
	t_philo	*philo;
	t_philo *new;
	t_philo	*end;

	i = 0;
	philo = NULL;
	end = NULL;
	while(i < dinning_rules->nb_philo)
	{
		new = create_philo(philo, dinning_rules);
		if (new == NULL)
			return ;
		else if (philo == NULL)
		{	
			philo = new;
			end = new;
		}
		else
		{
			end->next = new;
			new->prev = end;
			end = new;
		}
		i++;
	}
}
