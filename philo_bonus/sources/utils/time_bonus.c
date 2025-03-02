/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/02 11:53:41 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers_bonus.h"

time_t	current_time(void)
{
	struct timeval	start;
	long			time;

	gettimeofday(&start, NULL);
	time = start.tv_sec * 1000 + start.tv_usec / 1000;
	return (time);
}

time_t	step_timer(t_rules *rules)
{
	struct timeval	end;
	long			time;

	gettimeofday(&end, NULL);
	time = (end.tv_sec - rules->start.tv_sec) * 1000
		+ (end.tv_usec - rules->start.tv_usec) / 1000;
	if (time % 10 == 9)
		time += 1;
	return (time);
}
