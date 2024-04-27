/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 18:18:56 by rtamouss          #+#    #+#             */
/*   Updated: 2024/04/27 19:36:53 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (printf("gettimeofday failed\n"), 0);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(100);
	return (0);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	res;

	i = 0;
	res = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - 48);
		i++;
	}
	res *= sign;
	return (res);
}

int	ft_check_if_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] >= '0' && str[i] <= '9')
			|| ((str[i] == '+' || str[i] == '-')
				&& (str[i + 1] >= '0' && str[i + 1] <= '9')))
			i++;
		else
			return (0);
	}
	return (1);
}

int	ft_parsing(int ac, char **av)
{
	int	i;

	i = 1;
	if (ft_atoi(av[ac - 1]) == 0 && ac == 6)
		return (0);
	while (i < ac)
	{
		if (ft_check_if_string(av[i]) == 0)
			return (printf("Error: Arguments must contain only digits.\n"), 0);
		if (ft_atoi(av[i]) <= 0)
		{
			printf("Error\n");
			if (av[1] == 0)
				return (printf("You must have at least one philosopher.\n"), 0);
			else
				return (printf("Invalid args! Try to enter valid args!\n"), 0);
		}
		if (ft_atoi(av[1]) > 200)
			return (printf("The entered philosopher count too big!\n"), 0);
		i++;
	}
	return (1);
}

int	ft_print(t_philo *philo, char *str, char *color)
{
	pthread_mutex_lock(&philo->data->dead_lock);
	if (philo->data->dead == 1)
	{
		pthread_mutex_unlock(&philo->data->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead_lock);
	pthread_mutex_lock(&philo->data->print);
	printf("%s%ld %d %s\n%s", color,
		get_current_time() - philo->start, philo->id + 1, str, DEFAULT);
	pthread_mutex_unlock(&philo->data->print);
	return (0);
}

void	check_if_all_ate_by_monitor(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->data->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].data->nb_meals_eaten_mutex);
		if (philos[i].data->num_times_to_eat != -1
			&& philos[i].nb_meals_eaten >= philos[i].data->num_times_to_eat)
		{
			pthread_mutex_lock(&philos[i].data->dead_lock);
			philos->eaten++;
			pthread_mutex_unlock(&philos[i].data->dead_lock);
		}
		pthread_mutex_unlock(&philos[i].data->nb_meals_eaten_mutex);
		i++;
	}
}

int	check_if_dead_by_monitor(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->data->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].data->last_meal_mutex);
		if ((get_current_time() - philos[i].last_meal
				> philos[i].data->time_to_die))
		{
			if (ft_print(&philos[i], "died", RED) == 1)
			{
				pthread_mutex_unlock(&philos[i].data->last_meal_mutex);
				return (1);
			}
			pthread_mutex_lock(&philos[i].data->dead_lock);
			// *(philos[i].data->dead) = 1;
			philos[i].data->dead = 1;
			pthread_mutex_unlock(&philos[i].data->dead_lock);
			pthread_mutex_unlock(&philos[i].data->last_meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&philos[i].data->last_meal_mutex);
		i++;
	}
	return (0);
}

int	monitor(t_philo *philos)
{
	while (1)
	{
		if (check_if_dead_by_monitor(philos) == 1)
			return (1);
		philos->eaten = 0;
		check_if_all_ate_by_monitor(philos);
		if (philos->eaten == philos->data->num_of_philos)
		{
			pthread_mutex_lock(&philos->data->dead_lock);
			philos->data->dead = 1;
			pthread_mutex_unlock(&philos->data->dead_lock);
		}
	}
	return (0);
}

int	check_if_dead(t_data *data)
{
	int	stop;

	pthread_mutex_lock (&data->dead_lock);
	stop = data->dead;
	pthread_mutex_unlock (&data->dead_lock);
	while (stop != 0)
	{
		pthread_mutex_lock (&data->dead_lock);
		stop = data->dead;
		pthread_mutex_unlock (&data->dead_lock);
	}
	return (1);
}

int	start_routine(t_philo *philo)
{
	pthread_mutex_lock (philo->l_fork);
	if (ft_print(philo, "has taken a fork", YELLOW) == 1)
		return (pthread_mutex_unlock (philo->l_fork), 1);
	if (philo->data->num_of_philos == 1)
	{
		if (check_if_dead(philo->data) == 1)
			return (pthread_mutex_unlock(philo->l_fork), 1);
	}
	pthread_mutex_lock(philo->r_fork);
	if (ft_print(philo, "has taken a fork", YELLOW) == 1)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return (1);
	}
	if (ft_print(philo, "is eating", GREEN) == 1)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return (1);
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (start_routine(philo) == 1)
			return (NULL);
		pthread_mutex_lock(&philo->data->last_meal_mutex);
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(&philo->data->last_meal_mutex);
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_lock(&philo->data->nb_meals_eaten_mutex);
		philo->nb_meals_eaten++;
		pthread_mutex_unlock(&philo->data->nb_meals_eaten_mutex);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		if (ft_print(philo, "is sleeping", WHITE) == 1)
			return (NULL);
		ft_usleep(philo->data->time_to_sleep);
		if (ft_print(philo, "is thinking", CYAN) == 1)
			return (NULL);
		ft_usleep((philo->data->time_to_die - (get_current_time()
					- philo->last_meal)) / 2);
	}
	return (NULL);
}

void	print_usage(void)
{
	printf (RED "Error: Invalid arguments!\n" YELLOW
		"Usage: ./philosophers <number_of_philosophers> "
		"<time_to_die (ms)> <time_to_eat (ms)> <time_to_sleep (ms)> "
		"[<number_of_times_each_philosopher_must_eat>]!\n" DEFAULT
		YELLOW "- <number_of_philosophers>:" GREEN " Number of philosophers.\n"
		YELLOW "- <time_to_die(ms)>:" GREEN " Time to die if not eating.\n"
		YELLOW "- <time_to_eat(ms)>:" GREEN " Time it takes to eat.\n"
		YELLOW "- <time_to_sleep(ms)>:" GREEN " Time to sleeping.\n"
		YELLOW "- [<number_of_times_each_philosopher_must_eat>] (optional):\n"
		RED "Please try again with the correct arguments.\n" DEFAULT);
}

void init_data(t_data *data, char **av)
{
	data->num_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5] != NULL)
		data->num_times_to_eat = ft_atoi(av[5]);
	else
		data->num_times_to_eat = -1;
	data->dead = 0;
}

int init_mutexes(t_data *data, pthread_mutex_t *forks, int num_philos)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0)
		return (printf("mutex_init failed\n"), 1);
	if (pthread_mutex_init(&data->print, NULL) != 0)
		return (printf("Mutex Init Failed\n"), 1);
	if (pthread_mutex_init(&data->nb_meals_eaten_mutex, NULL) != 0)
		return (printf("Mutex Init Failed\n"), 1);
	if (pthread_mutex_init(&data->last_meal_mutex, NULL) != 0)
		return (printf("Mutex Init Failed\n"), 1);
	while (i < num_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (printf("Mutex Init Failed\n"), 1);
		i++;
	}
	return (0);
}

int initialize_philos_and_start(t_philo *philos, t_data *data, pthread_mutex_t *forks, int num_philos)
{
	int i;

	i = 0;
	while (i < num_philos)
	{
		philos[i].id = i;
		philos[i].last_meal = get_current_time();
		philos[i].data = data;
		philos[i].nb_meals_eaten = 0;
		if (philos[i].id % 2)
		{
			philos[i].l_fork = &forks[i];
			philos[i].r_fork = &forks[(i + 1) % num_philos];
		}
		else
		{
			philos[i].r_fork = &forks[i];
			philos[i].l_fork = &forks[(i + 1) % num_philos];
		}
		philos[i].start = get_current_time();
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			return (printf("pthread_create failed\n"), 1);
		i++;
	}
	return (0);
}
int ft_free_and_join(t_data *data, t_philo *philos)
{
	int	i;

	if (data->dead == 1)
	{
		i = 0;
		while (i < data->num_of_philos)
		{
			if (pthread_join(philos[i].thread, NULL) != 0)
			{
				printf("pthread_join failed\n");
				return (1);
			}
			i++;
		}
		if (pthread_mutex_destroy(&data->print) != 0)
			printf("Failed to destroy print mutex\n");
		if (pthread_mutex_destroy(&data->last_meal_mutex) != 0)
			printf("Failed to destroy last meal mutex\n");
		if (pthread_mutex_destroy(&data->dead_lock) != 0)
			printf("Failed to destroy dead_lock mutex\n");
		free(philos);
		free(data->forks);
		return (1);
	}
	return (0);
}
int	main(int ac, char **av)
{
	t_philo			*philos;
	t_data			data;
	pthread_mutex_t	*forks;
	int				num_philos;

	if (ac != 5 && ac != 6)
		return (print_usage(), 0);
	if (ft_parsing(ac, av) == 0)
		return (0);
	num_philos = ft_atoi(av[1]);
	philos = malloc(sizeof(t_philo) * num_philos);
	if (philos == NULL)
		return (printf("Error: Memory allocation failed!\n"), 1);
	forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	if (forks == NULL)
		return (printf("Error: Memory allocation failed!\n"), 1);
	init_data(&data, av);
	if (init_mutexes(&data, forks, num_philos) == 1)
		return (0);
	data.forks = forks;
	if (initialize_philos_and_start(philos, &data, forks ,num_philos) == 1)
		return (0);
	monitor(philos);
	ft_free_and_join(&data, philos);
	return (0);
}
