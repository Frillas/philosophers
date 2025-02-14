/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/14 11:10:40 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

int	check_last_meal(t_philo *philo, t_rules *rules)
{
	if (current_time() - philo->last_meal_time > rules->time_to_die)
	{ 
		philo->status = DEAD;
		print_status(philo);
		return (1);
	}
	return (0);
}

long	current_time(void)
{
	struct	timeval	start;
	long			time;

	gettimeofday(&start, NULL);
	time = start.tv_sec * 1000 + start.tv_usec / 1000;
	return (time);
}

long	step_timer(t_rules *rules)
{
	struct timeval	end;
	long			time;

	gettimeofday(&end, NULL);
	time = (end.tv_sec - rules->start.tv_sec) * 1000
		+ (end.tv_usec - rules->start.tv_usec) / 1000;
	return (time);
}
