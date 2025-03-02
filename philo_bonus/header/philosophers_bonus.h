/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroullea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:46:42 by aroullea          #+#    #+#             */
/*   Updated: 2025/03/02 10:40:25 by aroullea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

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
	SATIATED,
	TAKES_FORK,
	SLEEP,
	THINK,
	EAT,
	DEAD
}	t_status;

typedef struct s_rules
{
	long				nb_philo;
	long				created_philo;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	long				meals_per_philo;
	struct timeval		start;
	sem_t				*sem_fork;
	sem_t				*sem_status;
	sem_t				*sem_die;
	sem_t				*sem_eat;
	sem_t				*sem_end_diner;
	pthread_t			moni;
	pid_t				*fork_id;
	t_bool				end_dinner;
	t_bool				error;
}	t_rules;

typedef struct s_philo
{
	long				index;
	t_status			status;
	time_t				last_meal_time;
	long				meals_eaten;
	t_rules				*lst_rules;
	struct s_philo		*right;
	struct s_philo		*left;
}	t_philo;

//parsing_bonus.c
void	parsing(int argc, char *argv[], t_rules *dining_rules);
//utils_bonus.c
void	print_status(t_philo *philo);
t_bool	atol_valid(const char *s, long *value, t_bool res);
size_t	ft_strlen(const char *s);
//split_bonus.c
char	**split(char const *s, char c);
//error_bonus.c
t_bool	is_empty(char *str);
t_bool	check_arg(t_rules *dining_rules);
void	error_msg(char str[], t_rules *dining_rules);
//free_bonus.c
void	free_2d(char **result);
void	free_struct(t_philo *philo, int nb_philo);
void	close_semaphores(t_rules *rules);
void	err_init_semaphores(int error, t_rules *rules);
//philosophers_bonus.c
int		start_philo(t_rules *dining_rules);
//parent_bonus.c
void	handle_forks(t_rules *rules, t_philo *philo);
//serve_food_bonus.c
void	serve_food(t_rules *dining_rules, t_philo *philo, pid_t *fork_id);
//supervise_bonus.c
void	*supervise(void *arg);
//check_philosopher_bonus.c
void	death_and_meal_threads(t_rules *rules);
//time_bonus.c
time_t	current_time(void);
time_t	step_timer(t_rules *rules);
#endif
