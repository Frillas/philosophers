/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:19:57 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/05 19:08:20 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	initialize_rules(t_rules *dinning_rules)
{
	dinning_rules->nb_philo = -1;
	dinning_rules->time_to_die = -1;
	dinning_rules->time_to_eat = -1;
	dinning_rules->time_to_sleep = -1;
	dinning_rules->meals_per_philo = -1;
	dinning_rules->error = FALSE;
}

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
	return (EXIT_SUCCESS);
}
