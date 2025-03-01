/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:21:23 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/13 13:55:41 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	add_rules(long int value, t_rules *dining_rules)
{
	static int	i;
	int			test;

	test = value * 100;
	if ((i > 0 && i < 4) && (test < 0))
	{
		error_msg("Invalid argument\n", dining_rules);
		return ;
	}
	if (i == 0)
		dining_rules->nb_philo = value;
	else if (i == 1)
		dining_rules->time_to_die = value;
	else if (i == 2)
		dining_rules->time_to_eat = value * 1000;
	else if (i == 3)
		dining_rules->time_to_sleep = value * 1000;
	else if (i == 4)
		dining_rules->meals_per_philo = value;
	else if (i > 4)
		error_msg("Too many arguments\n", dining_rules);
	i++;
}

static void	build_list(char **result, t_rules *dining_rules)
{
	int			i;
	long int	value;
	t_bool		res;

	i = 0;
	res = FALSE;
	while ((result[i] != NULL) && (dining_rules->error == FALSE))
	{
		value = 0;
		res = atoi_valid(result[i], &value, res);
		if (!res || ((value > 2417483647) || (value < 0)))
		{
			error_msg("Invalid argument\n", dining_rules);
			break ;
		}
		add_rules(value, dining_rules);
		i++;
	}
	free_2d(result);
}

void	parsing(int argc, char *argv[], t_rules *dining_rules)
{
	int			i;
	char		**result;

	i = 1;
	while ((i < argc) && (dining_rules->error == FALSE))
	{
		if (is_empty(argv[i]))
		{
			dining_rules->error = TRUE;
			return ;
		}
		result = split(argv[i], ' ');
		if (result == NULL)
		{
			error_msg("Memory allocation failed\n", dining_rules);
			return ;
		}
		else
			build_list(result, dining_rules);
		i++;
	}
}
