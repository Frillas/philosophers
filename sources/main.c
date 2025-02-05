/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:19:57 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/05 16:40:36 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

int	main(int argc, char *argv[])
{
	t_rules	dinning_rules;

	if ((argc < 5) || (argc > 6))
		return (EXIT_FAILURE);
	dinning_rules = parsing(argc, argv);
	if (dinning_rules.error == TRUE)
		return (EXIT_FAILURE);
	printf("nombre de philosophe : %d\n", dinning_rules.nb_philo);
	printf("time to die = %d\n", dinning_rules.time_to_die);
	printf("time to eat = %d\n", dinning_rules.time_to_eat);
	printf("time to sleep = %d\n", dinning_rules.time_to_sleep);
	if (dinning_rules.meals_per_philo != 0)
		printf("meals_per_philo = %d\n", dinning_rules.meals_per_philo);
	return (EXIT_SUCCESS);
}
