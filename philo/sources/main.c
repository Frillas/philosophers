/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:19:57 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/12 17:23:15 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	initialize_rules(t_rules *dining_rules)
{
	pthread_mutex_init(&dining_rules->print_lock, NULL);
	dining_rules->nb_philo = -1;
	dining_rules->time_to_die = -1;
	dining_rules->time_to_eat = -1;
	dining_rules->time_to_sleep = -1;
	dining_rules->meals_per_philo = -1;
	dining_rules->error = FALSE;
}

/*static void	print_rules(t_rules *dining_rules)
{
	printf("nb philo : %d\n", dining_rules->nb_philo);
	printf("time to die : %d\n", dining_rules->time_to_die);
	printf("time to eat : %d\n", dining_rules->time_to_eat);
	printf("time to sleep : %d\n", dining_rules->time_to_sleep);
	if (dining_rules->meals_per_philo != -1)
		printf("meals per philo : %d\n", dining_rules->meals_per_philo);
}*/

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
	if (start_philo(&dining_rules) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
