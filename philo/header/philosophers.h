/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:46:42 by aroullea          #+#    #+#             */
/*   Updated: 2025/02/24 18:37:11 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum s_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef enum s_status
{
	DEAD,
	ERROR,
	UNCHANGED,
	TAKES_FORK,
	SLEEP,
	THINK,
	EAT,
}	t_status;

typedef struct s_rules
{
	long				nb_philo;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	long				meals_per_philo;
	struct timeval		start;
	struct timeval		end;
	pthread_t			thread_id;
	pthread_mutex_t		status_lock;
	t_bool				error;
}	t_rules;

typedef struct s_philo
{
	long				index;
	t_status			status;
	pthread_mutex_t		mutex;
	long				last_meal_time;
	long				meals_eaten;
	struct s_rules		*lst_rules;
	struct s_philo		*right;
	struct s_philo		*left;
}	t_philo;

//parsing.c
void	parsing(int argc, char *argv[], t_rules *dining_rules);
//philosophers.c
int		start_philo(t_rules *dining_rules);
//serve_food.c
void	*serve_food(void *arg);
//supervise.c
void	*supervise(void *arg);
//threads.c
int		handle_threads(t_rules *rules, t_philo *philo, pthread_t *thread_id);
//threads_error.c
void 	err_thread(t_philo *philo, pthread_t *thread, long created);
//error.c
t_bool	is_empty(char *str);
t_bool	check_arg(t_rules *dining_rules);
void	error_msg(char str[], t_rules *dining_rules);
//split.c
char	**split(char const *s, char c);
//time.c
time_t	current_time(void);
time_t	step_timer(t_rules *rules);
//utils
void	print_status(t_philo *philo);
t_bool	atoll_valid(const char *s, long long *value, t_bool res);
size_t	ft_strlen(const char *s);
void	free_2d(char **result);
void	free_struct(t_philo *philo, int nb_philo);
#endif
