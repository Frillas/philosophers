/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:45:48 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/24 18:34:33 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

void	err_thread(t_philo *philo, pthread_t *thread, long created)
{
	int		nb_thread_waited;
	t_philo	*current;

	nb_thread_waited = 0;
	current = philo;
	while (nb_thread_waited < created - 1)
	{
		if (pthread_join(thread[nb_thread_waited], NULL) != 0)
			write(2, "thread wait error\n", 18);
		current = current->right;
		nb_thread_waited++;
	}
}
