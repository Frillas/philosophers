/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:46:42 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/05 18:48:28 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# define TRUE 1
# define FALSE 0

typedef int	t_bool;
typedef struct s_rules
{
	int		nb_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		meals_per_philo;
	t_bool	error;
}	t_rules;
//parsing.c
void	parsing(int argc, char *argv[], t_rules *dinning_rules);
//utils
t_bool	atoi_valid(const char *s, long int *value, t_bool res);
void	free_2d(char **result);
size_t	ft_strlen(const char *s);
//split.c
char	**split(char const *s, char c);
//error.c
t_bool	is_empty(char *str);
t_bool	check_arg(t_rules *dinning_rules);
void	error_msg(char str[], t_rules *dinning_rules);

#endif
