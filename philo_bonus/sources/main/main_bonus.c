/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:19:57 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/28 12:27:50 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static int	init_semaphores(t_rules *rules)
{
	int	tot;

	tot = rules->nb_philo;
	rules->sem_fork = sem_open("/fork_sem", O_CREAT | O_EXCL, 0644, tot);
	if (rules->sem_fork == SEM_FAILED)
		return (EXIT_FAILURE);
	rules->sem_status = sem_open("/status_sem", O_CREAT | O_EXCL, 0644, 1);
	if (rules->sem_status == SEM_FAILED)
		err_init_semaphores(1, rules);
	rules->sem_die = sem_open("/die_sem", O_CREAT | O_EXCL, 0644, 0);
	if (rules->sem_die == SEM_FAILED)
		err_init_semaphores(2, rules);
	rules->sem_eat = sem_open("/eat_sem", O_CREAT | O_EXCL, 0644, 0);
	if (rules->sem_eat == SEM_FAILED)
		err_init_semaphores(3, rules);
	rules->sem_end = sem_open("/end_sem", O_CREAT | O_EXCL, 0644, 1);
	if (rules->sem_end == SEM_FAILED)
		err_init_semaphores(4, rules);
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
