/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_spawn_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:32:34 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/02 10:31:13 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	wait_philo(t_philo *philo, t_rules *rules)
{
	int			philo_count;
	int			status;
	int			exit_code;

	philo_count = 0;
	exit_code = 0;
	while (philo_count < rules->created_philo)
	{
		if (waitpid(-1, &status, 0) == -1)
			error_msg("waitpid error\n", rules);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
			exit_code = 1;
		philo_count++;
	}
	if (rules->error == TRUE)
		exit_code = 1;
	close_semaphores(rules);
	free(rules->fork_id);
	free_struct(philo, rules->nb_philo);
	exit (exit_code);
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
	rules->created_philo = philo_created;
	death_and_meal_threads(rules);
	wait_philo(philo, rules);
}
