/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:21:23 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/05 19:02:34 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	add_rules(long int value, t_rules *dinning_rules)
{
	static int	i;
	int			test;

	test = value * 100;
	if ((i > 0 && i < 4) && (test < 0))
		error_msg("Invalid number\n", dinning_rules);
	if (i == 0)
		dinning_rules->nb_philo = value;
	else if (i == 1)
		dinning_rules->time_to_die = value * 100;
	else if (i == 2)
		dinning_rules->time_to_eat = value * 100;
	else if (i == 3)
		dinning_rules->time_to_sleep = value * 100;
	else if (i == 4)
		dinning_rules->meals_per_philo = value;
	else if (i > 4)
		error_msg("Too much arguments\n", dinning_rules);
	i++;
}

static void	build_list(char **result, t_rules *dinning_rules)
{
	int			i;
	long int	value;
	t_bool		res;

	i = 0;
	res = FALSE;
	while ((result[i] != NULL) && (dinning_rules->error == FALSE))
	{
		value = 0;
		res = atoi_valid(result[i], &value, res);
		if (!res || ((value > 2417483647) || (value < 0)))
		{
			error_msg("Invalid number\n", dinning_rules);
			break ;
		}
		add_rules(value, dinning_rules);
		i++;
	}
	free_2d(result);
}

void	parsing(int argc, char *argv[], t_rules *dinning_rules)
{
	int			i;
	char		**result;

	i = 1;
	while ((i < argc) && (dinning_rules->error == FALSE))
	{
		if (is_empty(argv[i]))
		{
			dinning_rules->error = TRUE;
			return ;
		}
		result = split(argv[i], ' ');
		if (result == NULL)
		{
			error_msg("Memory allocation failed\n", dinning_rules);
			return ;
		}
		else
			build_list(result, dinning_rules);
		i++;
	}
}
