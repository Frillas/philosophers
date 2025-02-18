/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:54:18 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/18 12:05:58 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

void	handle_forks(t_rules *dining_rules, t_philo *philo, int *fork_id)
{
	int	i;

	i = 0;
	gettimeofday(&dining_rules->start, NULL);
	while (i < dining_rules->nb_philo)
	{
		fork_id[i] = fork();
		if (fork_id[i] < 0)
	}
}
