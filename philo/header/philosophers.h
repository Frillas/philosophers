/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:46:42 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/07 12:05:22 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

typedef enum s_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef enum s_status
{
	SLEEP = 1,
	THINK,
	EAT
}	t_status;

typedef struct s_philo
{
	int					r_fork;
	t_status			status;
	struct s_philo		*right;
	struct s_philo		*left;
}	t_philo;

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
size_t	ft_strlen(const char *s);
void	free_2d(char **result);
void	free_struct(t_philo *philo, int nb_philo);
//split.c
char	**split(char const *s, char c);
//error.c
t_bool	is_empty(char *str);
t_bool	check_arg(t_rules *dinning_rules);
void	error_msg(char str[], t_rules *dinning_rules);
//philosophers.c
int		start_philo(t_rules *dinning_rules);
//dinner.c
int		start_dinner(t_rules *dinning_rules, t_philo *philo, pthread_t *thread_id);
#endif
