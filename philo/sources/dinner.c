/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:08:37 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/06 18:02:11 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

static void	*eating(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->status = EAT;
	philo->left->fork = 0;
	printf("philo 1 is eating\n");
	return (NULL);
}

void	start_dinner(t_rules *dinning_rules, t_philo *philo)
{
	pthread_t   t1;

	(void)dinning_rules;
	if (pthread_create(&t1, NULL, eating, philo) != 0)
		return ;
	if (pthread_join(t1, NULL) != 0)
		return ;
}
