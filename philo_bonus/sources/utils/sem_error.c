/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <aroullea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 08:08:07 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/22 08:24:57 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

void check_sem_wait(sem_t *sem_fork, t_philo *philo, pid_t *fork_id)
{
	if (sem_wait(sem_fork) != 0)
	{
		write(2, "sem_post error\n", 15);
		dining_rules = philo->lst_rules;
		free(fork_id);
		sem_close(dining_rules->sem_fork);
		sem_close(dining_rules->sem_status);
		free_struct(philo, dining_rules->nb_philo);
		exit(EXIT_FAILURE);
	}
}

void check_sem_post(sem_t *sem_fork, t_philo *philo, pid_t *fork_id)
{
	if (sem_post(sem_fork) != 0)
	{
		write(2, "sem_post error\n", 15);
		dining_rules = philo->lst_rules;
		free(fork_id);
		sem_close(dining_rules->sem_fork);
		sem_close(dining_rules->sem_status);
		free_struct(philo, dining_rules->nb_philo);
		exit(EXIT_FAILURE);
	}
}
