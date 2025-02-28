/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:32:34 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/28 09:05:44 by aroullea         ###   ########.fr       */
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

static void	*stop_philo(void *arg)
{
	t_rules	*rules;
	int		i;

	i = 0;
	rules = (t_rules *)arg;
	sem_wait(rules->sem_die);
	sem_wait(rules->sem_end);
	if (rules->end_dinner == TRUE)
	{
		sem_post(rules->sem_end);
		return (NULL);
	}
	sem_post(rules->sem_end);
	while (i < rules->nb_philo)
	{
		kill(rules->fork_id[i], SIGKILL);
		i++;
	}
	if (i == rules->nb_philo)
	{
		sem_wait(rules->sem_end);
		rules->end_dinner = TRUE;
		sem_post(rules->sem_end);
		sem_post(rules->sem_eat);
	}
	return (NULL);
}

static void	*big_belly(void *arg)
{
	t_rules	*rules;
	int		i;

	rules = (t_rules *)arg;
	i = 0;
	while (i < rules->nb_philo)
	{
		sem_wait(rules->sem_eat);
		sem_wait(rules->sem_end);
		if (rules->end_dinner == TRUE)
		{
			sem_post(rules->sem_end);
			return (NULL);
		}
		sem_post(rules->sem_end);
		i++;
	}
	if (i == rules->nb_philo)
	{
		sem_wait(rules->sem_end);
		rules->end_dinner = TRUE;
		sem_post(rules->sem_end);
		sem_post(rules->sem_die);
	}
	return (NULL);
}

void	handle_forks(t_rules *rules, t_philo *philo)
{
	int			philo_created;
	t_philo		*current;

	philo_created = 0;
	current = philo;
	rules->end_dinner = FALSE;
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
	if (rules->nb_philo > 1)
	{
		pthread_create(&rules->philo_death, NULL, stop_philo, (void *)rules);
		pthread_detach(rules->philo_death);
		pthread_create(&rules->philo_eat, NULL, big_belly, (void *)rules);
		pthread_detach(rules->philo_eat);
	}
	wait_child(philo, rules, rules->fork_id, philo_created);
}
