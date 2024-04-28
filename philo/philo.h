/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:26:55 by rtamouss          #+#    #+#             */
/*   Updated: 2024/04/27 22:48:36 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/time.h>

# define RED "\033[0;31m"
# define WHITE "\033[0;37m"
# define DEFAULT "\033[0m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"

typedef struct s_data
{
	int				num_of_philos;
	size_t			time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	int				dead;
	size_t			start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	nb_meals_eaten_mutex;
	pthread_mutex_t	last_meal_mutex;
}	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eaten;
	int				nb_meals_eaten;
	size_t			last_meal;
	int				finished;
	size_t			start;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	t_data			*data;
}	t_philo;

int		ft_atoi(const char *str);
int		ft_check_if_string(char *str);
int		ft_parsing(int ac, char **av);
void	print_usage(void);
void	check_if_all_ate_by_monitor(t_philo *philos);
int		check_if_dead_by_monitor(t_philo *philos);
int		monitor(t_philo *philos);
int		check_if_dead(t_data *data);
int		start_routine(t_philo *philo);
void	*routine(void *arg);
void	init_data(t_data *data, char **av);
int		init_mutexes(t_data *data, pthread_mutex_t *forks, int num_philos);
int		initialize_philos_and_start(t_philo *philos, t_data *data,
			int num_philos);
int		ft_free_and_join(t_data *data, t_philo *philos);
int		ft_print(t_philo *philo, char *str, char *color);
int		ft_usleep(size_t milliseconds);
size_t	get_current_time(void);

#endif
