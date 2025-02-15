/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:24:35 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/15 10:57:51 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

void	print_status(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->lst_rules;
	if (philo->status == TAKES_FORK)
		printf("%ld %d has taken a fork\n", step_timer(rules), philo->index);
	else if (philo->status == EAT)
		printf("%ld %d is eating\n", step_timer(rules), philo->index);
	else if (philo->status == SLEEP)
		printf("%ld %d is sleeping\n", step_timer(rules), philo->index);
	else if (philo->status == THINK)
		printf("%ld %d is thinking\n", step_timer(rules), philo->index);
	else if (philo->status == DEAD)
		printf("%ld %d died\n", step_timer(rules), philo->index);
}

t_bool	atoi_valid(const char *s, long int *value, t_bool res)
{
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	res = FALSE;
	while (s[i] == ' ' || (s[i] >= '\t' && s[i] <= '\r'))
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = sign * (-1);
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = TRUE;
		*value = (*value * 10) + (s[i++] - '0');
		if (*value < 0)
			return (FALSE);
	}
	*value = *value * sign;
	if ((res == TRUE) && (s[i] == '\0'))
		return (TRUE);
	return (FALSE);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i] != '\0')
		i++;
	return (i);
}

void	free_2d(char **result)
{
	int		i;

	i = 0;
	if (result != NULL)
	{
		while (result[i])
		{
			free(result[i]);
			i++;
		}
		free(result);
	}
}

void	free_struct(t_philo *philo, int nb_philo)
{
	t_philo	*current;
	int		i;

	i = 0;
	if (philo != NULL)
	{
		current = philo;
		while (i < nb_philo)
		{
			philo = philo->right;
			free(current);
			current = philo;
			i++;
		}
	}
}
