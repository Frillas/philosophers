/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:54:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/21 16:12:28 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void	eat_or_sleep(long long duration, t_philo *philo)
{
	long	runtime;

	runtime = 0;
	duration *= 1000;
	while (runtime < duration)
	{
		sem_wait(philo->lst_rules->sem_print);
		if (philo->status == DEAD)
		{
			sem_post(philo->lst_rules->sem_print);
			return ;
		}
		sem_post(philo->lst_rules->sem_print);
		usleep(10000);
		runtime += 10000;
	}
}

static int	check_status(t_philo *philo, t_status status)
{
	sem_wait(philo->lst_rules->sem_print);
	if (philo->status == DEAD)
	{
		sem_post(philo->lst_rules->sem_print);
		return (1);
	}
	else if (status != UNCHANGED)
	{
		philo->status = status;
		print_status(philo);
		if (philo->status == EAT)
			philo->meals_eaten++;
		sem_post(philo->lst_rules->sem_print);
		return (0);
	}
	sem_post(philo->lst_rules->sem_print);
	return (0);
}

static void	philo_set_state(t_philo *philo)
{
	check_status(philo, EAT);
	eat_or_sleep(philo->lst_rules->time_to_eat, philo);
	philo->last_meal_time = current_time();
	check_status(philo, SLEEP);
	sem_post(philo->lst_rules->sem_fork);
	sem_post(philo->lst_rules->sem_fork);
	eat_or_sleep(philo->lst_rules->time_to_sleep, philo);
	check_status(philo, THINK);
	sem_wait(philo->lst_rules->sem_print);
	if (philo->meals_eaten == philo->lst_rules->meals_per_philo)
		philo->status = DEAD;
	sem_post(philo->lst_rules->sem_print);
}

void	handle_exit(t_philo *philo, pid_t *fork_id, int result, pthread_t *moni)
{
	t_rules	*dining_rules;

	pthread_join(*moni, NULL);
	dining_rules = philo->lst_rules;
	free(fork_id);
	sem_close(dining_rules->sem_fork);
	sem_close(dining_rules->sem_print);
	free_struct(philo, dining_rules->nb_philo);
	exit(result);
}

void	handle_one(t_philo *philo, pid_t *fork_id, int result, pthread_t *moni)
{
	check_status(philo, TAKES_FORK);
	handle_exit(philo, fork_id, result, moni);
}

static void	serve_food(t_rules *dining_rules, t_philo *philo, pid_t *fork_id)
{
	pthread_t	moni;

	if (pthread_create(&moni, NULL, supervise, (void *)philo) != 0)
		handle_exit(philo, fork_id, 1, &moni);
	if (philo == philo->right)
		handle_one(philo, fork_id, 1, &moni);
	while (1)
	{
		if (check_status(philo, UNCHANGED) == 1)
			handle_exit(philo, fork_id, 0, &moni);
		sem_wait(dining_rules->sem_fork);
		check_status(philo, TAKES_FORK);
		sem_wait(dining_rules->sem_fork);
		if (check_status(philo, TAKES_FORK) == 1)
		{
			sem_post(dining_rules->sem_fork);
			sem_post(dining_rules->sem_fork);
			handle_exit(philo, fork_id, 0, &moni);
		}
		philo_set_state(philo);
	}
}

static void	wait_child(t_philo *philo, t_rules *dining_rules, int *fork_id)
{
	int	i;
	int	status;

	i = 0;
	while (i < dining_rules->nb_philo)
	{
		waitpid(fork_id[i], &status, 0);
		i++;
	}
	free(fork_id);
	sem_close(dining_rules->sem_fork);
	sem_close(dining_rules->sem_print);
	sem_unlink("/fork_sem");
	sem_unlink("/print_sem");
	free_struct(philo, dining_rules->nb_philo);
	exit(EXIT_SUCCESS);
}

int	handle_forks(t_rules *dining_rules, t_philo *philo, pid_t *fork_id)
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
			fork_error_exit(fork_id);
		else if (fork_id[i] == 0)
			serve_food(dining_rules, current, fork_id);
		current = current->right;
		i++;
	}
	wait_child(philo, dining_rules, fork_id);
	return (EXIT_SUCCESS);
}
