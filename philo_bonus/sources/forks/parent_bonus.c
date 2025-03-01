/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:32:34 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/01 15:48:46 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	wait_child(t_philo *philo, t_rules *rules, int *fork_id, int nb)
{
	int			philo_waited;
	int			status;
	int			returned;
	int			err_code;

	philo_waited = 0;
	returned = 0;
	err_code = 0;
	while (philo_waited < nb)
	{
		if (waitpid(-1, &status, 0) == -1)
			error_msg("waitpid error\n", rules);
		if (WIFEXITED(status) == 1)
			returned = WEXITSTATUS(status);
		if (returned == 1)
			err_code = 1;
		philo_waited++;
	}
	if (rules->error == TRUE)
		err_code = 1;
	close_semaphores(rules);
	free(fork_id);
	free_struct(philo, rules->nb_philo);
	exit (err_code);
}

void	handle_forks(t_rules *rules, t_philo *philo)
{
	long		philo_created;
	t_philo		*current;

	philo_created = 0;
	current = philo;
	gettimeofday(&rules->start, NULL);
	while (philo_created < rules->nb_philo)
	{
		rules->fork_id[philo_created] = fork();
		if (rules->fork_id[philo_created] < 0)
		{
			error_msg("fork error\n", rules);
			break ;
		}
		else if (rules->fork_id[philo_created] == 0)
			serve_food(rules, current, rules->fork_id);
		current = current->right;
		philo_created++;
	}
	death_and_meal_threads(rules, philo_created);
	wait_child(philo, rules, rules->fork_id, philo_created);
}
