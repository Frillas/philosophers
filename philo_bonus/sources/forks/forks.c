/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:54:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/19 17:22:53 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static void eat_or_sleep(long long duration)
{
	long	runtime;

	runtime = 0;
	duration *= 1000;
	while (runtime < duration)
	{
		usleep(10000);
		runtime += 10000;
	}
}

static int  check_status(t_philo *philo, t_status status)
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

static void philo_set_state(t_philo *philo)
{
	check_status(philo, EAT);
	eat_or_sleep(philo->lst_rules->time_to_eat);
	philo->last_meal_time = current_time();
	check_status(philo, SLEEP);
	sem_post(philo->lst_rules->sem_fork);
	sem_post(philo->lst_rules->sem_fork);
	eat_or_sleep(philo->lst_rules->time_to_sleep);
   	check_status(philo, THINK);
}

static void	serve_food(t_rules *dining_rules, t_philo *philo)
{
	pthread_t	moni;

	if (pthread_create(&moni, NULL, supervise, (void *)philo) != 0)
		exit(EXIT_FAILURE);
	if (pthread_detach(moni) != 0)
		exit(EXIT_FAILURE);
	while (1)
	{
		if (check_status(philo, UNCHANGED) == 1)
			exit(EXIT_SUCCESS);
		sem_wait(dining_rules->sem_fork);
		check_status(philo, TAKES_FORK);
		if (check_status(philo, TAKES_FORK) == 1)
		{
			sem_post(dining_rules->sem_fork);
			sem_post(dining_rules->sem_fork);
			exit(EXIT_SUCCESS);
		}	
		philo_set_state(philo);
	}
}

static void wait_child(t_rules *dining_rules, int *fork_id)
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
	exit(EXIT_SUCCESS);
}

int	handle_forks(t_rules *dining_rules, t_philo *philo, int *fork_id)
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
			serve_food(dining_rules, current);
		current = current->right;
		i++;
	}
	wait_child(dining_rules, fork_id);
	return (EXIT_SUCCESS);
}
