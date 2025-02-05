/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:21:23 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/05 13:26:15 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	add_rules(long int value, t_rules *dinning_rules)
{
	static int i;
	
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

static void *build_list(char **result, t_rules *dinning_rules)
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
		if (!res || ((value > 2417483647) || (value < -2147483648)))
		{
			std_error(result, dinning_rules);
			return (NULL);
		}
		add_rules(value, dinning_rules);
		i++;
	}
}

t_rules	parsing(int argc, char *argv[])
{
	int			i;
	char		**result;
	t_rules		dinning_rules;

	i = 1;
	dinning_rules = NULL;
	while (i < argc)
	{
		if (is_empty(argv[i]))
		{
			write(STDERR_FILENO, "empty argument\n", 15);
			std_error(NULL, dinning_rules);
			return (NULL);
		}
		result = split(argv[i], ' ');
		if (result == NULL)
			return (NULL);
		else
		{
			dinning_rules = build_list(result, dinning_rules);
			if (dinning_rules == NULL)
				return (NULL);
		{
		i++;
	}
	return (dinning_rules);
}
