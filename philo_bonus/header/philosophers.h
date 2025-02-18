/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:46:42 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/18 17:37:56 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <semaphore.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>

typedef enum s_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef enum s_status
{
	UNCHANGED,
	TAKES_FORK,
	SLEEP,
	THINK,
	EAT,
	DEAD
}	t_status;

typedef struct s_rules
{
	int					nb_philo;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				meals_per_philo;
	struct timeval		start;
	struct timeval		end;
	sem_t				*sem_fork;
	sem_t				*sem_print;
	t_bool				error;
}	t_rules;

typedef struct s_philo
{
	int					index;
	t_status			status;
	long				last_meal_time;
	long				meals_eaten;
	struct s_rules		*lst_rules;
	struct s_philo		*right;
	struct s_philo		*left;
}	t_philo;

//parsing.c
void	parsing(int argc, char *argv[], t_rules *dining_rules);
//utils
void	print_status(t_philo *philo);
t_bool	atoll_valid(const char *s, long long *value, t_bool res);
size_t	ft_strlen(const char *s);
void	free_2d(char **result);
void	free_struct(t_philo *philo, int nb_philo);
//split.c
char	**split(char const *s, char c);
//error.c
t_bool	is_empty(char *str);
t_bool	check_arg(t_rules *dining_rules);
void	error_msg(char str[], t_rules *dining_rules);
void    fork_error_exit(int *fork_id);
//philosophers.c
int		start_philo(t_rules *dining_rules);
//forks.c
int		handle_forks(t_rules *rules, t_philo *philo, int *thread_id);
//supervise.c
void    *supervise(void *arg);
//time.c
long	current_time(void);
long	step_timer(t_rules *rules);
#endif
