/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:33:13 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/27 17:31:39 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

void	close_semaphores(t_rules *rules)
{
	sem_close(rules->sem_fork);
	sem_close(rules->sem_status);
	sem_close(rules->sem_die);
	sem_close(rules->sem_eat);
	sem_unlink("/fork_sem");
	sem_unlink("/status_sem");
	sem_unlink("/die_sem");
	sem_unlink("/eat_sem");
}

void	free_2d(char **result)
{
	int		i;

	i = 0;
	if (result != NULL)
	{
		while (result[i])
		{
			free(result[i]);
			i++;
		}
		free(result);
	}
}

void	free_struct(t_philo *philo, int nb_philo)
{
	t_philo	*current;
	int		i;

	i = 0;
	if (philo != NULL)
	{
		current = philo;
		while (i < nb_philo)
		{
			philo = philo->right;
			free(current);
			current = philo;
			i++;
		}
	}
}
