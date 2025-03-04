/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:33:13 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/04 16:41:50 by aroullea         ###   ########.fr       */
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

void	close_and_unlink(sem_t *sem_free, char *sem_name)
{
	sem_close(sem_free);
	sem_unlink(sem_name);
}

void	err_init_semaphores(int error, t_rules *rules)
{
	write(2, "Sem open error\n", 15);
	if (error >= 1)
		close_and_unlink(rules->sem_fork, "/fork_sem");
	if (error >= 2)
		close_and_unlink(rules->sem_status, "/status_sem");
	if (error >= 3)
		close_and_unlink(rules->sem_die, "/die_sem");
	if (error >= 4)
		close_and_unlink(rules->sem_eat, "/eat_sem");
	exit(EXIT_FAILURE);
}
