/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:20:20 by rtamouss          #+#    #+#             */
/*   Updated: 2024/04/27 22:36:33 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		usleep(500);
	return (0);
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

int	ft_free_and_join(t_data *data, t_philo *philos)
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
