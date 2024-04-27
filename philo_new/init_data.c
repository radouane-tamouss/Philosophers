/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:18:06 by rtamouss          #+#    #+#             */
/*   Updated: 2024/04/27 22:25:59 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, char **av)
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

int	init_mutexes(t_data *data, pthread_mutex_t *forks, int num_philos)
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

int	initialize_philos_and_start(t_philo *philos, t_data *data, int num_philos)
{
	int	i;

	i = 0;
	while (i < num_philos)
	{
		philos[i].id = i;
		philos[i].last_meal = get_current_time();
		philos[i].data = data;
		philos[i].nb_meals_eaten = 0;
		if (philos[i].id % 2)
		{
			philos[i].l_fork = &data->forks[i];
			philos[i].r_fork = &data->forks[(i + 1) % num_philos];
		}
		else
		{
			philos[i].r_fork = &data->forks[i];
			philos[i].l_fork = &data->forks[(i + 1) % num_philos];
		}
		philos[i].start = get_current_time();
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			return (printf("pthread_create failed\n"), 1);
		i++;
	}
	return (0);
}
