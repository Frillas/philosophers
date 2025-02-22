/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:32:34 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/22 15:10:38 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

static void	wait_child(t_philo *philo, t_rules *dining_rules, int *fork_id)
{
	int			i;
	pid_t		pid;
	int			status;

	i = 0;
	while (i < dining_rules->nb_philo)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			error_msg("waitpid error\n", dining_rules);
		if (WEXITSTATUS(status) == 1)
			status = 1;
		i++;
	}
	if (dining_rules->error == TRUE && status == 0)
		status = 1;
	free(fork_id);
	sem_close(dining_rules->sem_fork);
	sem_close(dining_rules->sem_status);
	sem_unlink("/fork_sem");
	sem_unlink("/print_sem");
	free_struct(philo, dining_rules->nb_philo);
	exit(status);
}

void	handle_forks(t_rules *dining_rules, t_philo *philo, pid_t *fork_id)
{
	int		i;
	t_philo	*current;

	i = 0;
	current = philo;
	gettimeofday(&dining_rules->start, NULL);
	while (i < dining_rules->nb_philo)
	{
		fork_id[i] = fork();
		if (fork_id[i] < 0)
		{
			error_msg("fork error\n", dining_rules);
			wait_child(philo, dining_rules, fork_id);
		}
		else if (fork_id[i] == 0)
			serve_food(dining_rules, current, fork_id);
		current = current->right;
		i++;
	}
	wait_child(philo, dining_rules, fork_id);
}
