/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:19:57 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/25 11:02:49 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

static int	initialize_rules(t_rules *dining_rules)
{
	if (pthread_mutex_init(&dining_rules->status_lock, NULL) != 0)
		return (EXIT_FAILURE);
	dining_rules->nb_philo = -1;
	dining_rules->time_to_die = -1;
	dining_rules->time_to_eat = -1;
	dining_rules->time_to_sleep = -1;
	dining_rules->meals_per_philo = -1;
	dining_rules->error = FALSE;
	return (EXIT_SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_rules	dining_rules;

	if (argc > 6)
	{
		error_msg("Too much arguments\n", &dining_rules);
		return (EXIT_FAILURE);
	}
	if (initialize_rules(&dining_rules) != 0)
		return (EXIT_FAILURE);
	parsing(argc, argv, &dining_rules);
	if (dining_rules.error == TRUE || !(check_arg(&dining_rules)))
	{
		if (pthread_mutex_destroy(&dining_rules.status_lock) != 0)
			write(2, "mutex destroy error\n", 20);
		return (EXIT_FAILURE);
	}
	if (dining_rules.meals_per_philo == 0)
	{
		if (pthread_mutex_destroy(&dining_rules.status_lock) != 0)
			write(2, "mutex destroy error\n", 20);
		return (EXIT_SUCCESS);
	}
	if (handle_philo(&dining_rules) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
