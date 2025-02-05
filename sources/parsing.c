/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:21:23 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/05 16:40:21 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	add_rules(long int value, t_rules *dinning_rules)
{
	static int	i;

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
	i++;
}

static void	build_list(char **result, t_rules *dinning_rules)
{
	int			i;
	long int	value;
	t_bool		res;

	i = 0;
	res = FALSE;
	while (result[i] != NULL)
	{
		value = 0;
		res = atoi_valid(result[i], &value, res);
		if (!res || ((value > 2417483647) || (value < 0)))
		{
			free_2d(result);
			dinning_rules->error = TRUE;
			return ;
		}
		add_rules(value, dinning_rules);
		i++;
	}
	free_2d(result);
}

t_rules	parsing(int argc, char *argv[])
{
	int			i;
	char		**result;
	t_rules		dinning_rules;

	i = 1;
	dinning_rules.error = FALSE;
	dinning_rules.meals_per_philo = 0;
	while (i < argc)
	{
		if (is_empty(argv[i]))
		{
			dinning_rules.error = TRUE;
			break ;
		}
		result = split(argv[i], ' ');
		if (result == NULL)
		{
			dinning_rules.error = TRUE;
			break ;
		}
		else
		{
			build_list(result, &dinning_rules);
			if (dinning_rules.error == TRUE)
				break ;
		}
		i++;
	}
	return (dinning_rules);
}
