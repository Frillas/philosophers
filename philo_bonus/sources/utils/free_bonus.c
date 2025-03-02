/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:33:13 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/02 12:51:58 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

void	close_semaphores(t_rules *rules, int child)
{
	sem_close(rules->sem_fork);
	sem_close(rules->sem_status);
	sem_close(rules->sem_die);
	sem_close(rules->sem_eat);
	sem_close(rules->sem_end_diner);
	if (child == 1)
		return ;
	sem_unlink("/fork_sem");
	sem_unlink("/status_sem");
	sem_unlink("/die_sem");
	sem_unlink("/eat_sem");
	sem_unlink("/end_sem");
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

void	err_init_semaphores(int error, t_rules *rules)
{
	write(2, "Sem open error\n", 15);
	if (error >= 1)
	{
		sem_close(rules->sem_fork);
		sem_unlink("/fork_sem");
	}
	if (error >= 2)
	{
		sem_close(rules->sem_status);
		sem_unlink("/status_sem");
	}
	if (error >= 3)
	{
		sem_close(rules->sem_die);
		sem_unlink("/die_sem");
	}
	if (error >= 4)
	{
		sem_close(rules->sem_eat);
		sem_unlink("/eat_sem");
	}
	exit(EXIT_FAILURE);
}
