/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:19:57 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/27 11:27:24 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static int	init_semaphores(t_rules *rules)
{
	rules->sem_fork = sem_open("/fork_sem", O_CREAT, 0666, rules->nb_philo);
	if (rules->sem_fork == SEM_FAILED)
		return (EXIT_FAILURE);
	rules->sem_status = sem_open("/status_sem", O_CREAT, 0666, 1);
	if (rules->sem_status == SEM_FAILED)
	{
		sem_close(rules->sem_fork);
		sem_unlink("/fork_sem");
		return (EXIT_FAILURE);
	}
	rules->sem_die = sem_open("/die_sem", O_CREAT, 0666, 0);
	return (EXIT_SUCCESS);
}

static void	initialize_rules(t_rules *dining_rules)
{
	dining_rules->nb_philo = -1;
	dining_rules->time_to_die = -1;
	dining_rules->time_to_eat = -1;
	dining_rules->time_to_sleep = -1;
	dining_rules->meals_per_philo = -1;
	dining_rules->error = FALSE;
}

int	main(int argc, char *argv[])
{
	t_rules	dining_rules;

	if (argc > 6)
	{
		error_msg("Too much arguments\n", &dining_rules);
		return (EXIT_FAILURE);
	}
	initialize_rules(&dining_rules);
	parsing(argc, argv, &dining_rules);
	if (dining_rules.error == TRUE || !(check_arg(&dining_rules)))
		return (EXIT_FAILURE);
	if (dining_rules.meals_per_philo == 0)
		return (EXIT_SUCCESS);
	if (init_semaphores(&dining_rules) != 0)
		return (EXIT_FAILURE);
	if (start_philo(&dining_rules) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
