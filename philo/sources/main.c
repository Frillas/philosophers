/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:19:57 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/12 12:01:29 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	initialize_rules(t_rules *dinning_rules)
{
	pthread_mutex_init(&dinning_rules->print_lock, NULL);
	dinning_rules->nb_philo = -1;
	dinning_rules->time_to_die = -1;
	dinning_rules->time_to_eat = -1;
	dinning_rules->time_to_sleep = -1;
	dinning_rules->meals_per_philo = -1;
	dinning_rules->error = FALSE;
}

/*static void	print_rules(t_rules *dinning_rules)
{
	printf("nb philo : %d\n", dinning_rules->nb_philo);
	printf("time to die : %d\n", dinning_rules->time_to_die);
	printf("time to eat : %d\n", dinning_rules->time_to_eat);
	printf("time to sleep : %d\n", dinning_rules->time_to_sleep);
	if (dinning_rules->meals_per_philo != -1)
		printf("meals per philo : %d\n", dinning_rules->meals_per_philo);
}*/

int	main(int argc, char *argv[])
{
	t_rules	dinning_rules;

	if (argc > 6)
	{
		error_msg("Too much arguments\n", &dinning_rules);
		return (EXIT_FAILURE);
	}
	initialize_rules(&dinning_rules);
	parsing(argc, argv, &dinning_rules);
	if (dinning_rules.error == TRUE || !(check_arg(&dinning_rules)))
		return (EXIT_FAILURE);
	if (start_philo(&dinning_rules) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
