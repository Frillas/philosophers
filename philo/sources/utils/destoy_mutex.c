/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destoy_mutex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <aroullea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:38:13 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/09 11:40:37 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/philosophers.h"

t_bool	destroy_mutex(pthread_mutex_t *mutex)
{
	t_bool	error;

	error = FALSE;
	if (pthread_mutex_destroy(mutex) != 0)
	{
		write(2, "mutex destroy error\n", 20);
		error = TRUE;
	}
	return (error);
}
