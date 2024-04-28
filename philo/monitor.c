/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:14:46 by rtamouss          #+#    #+#             */
/*   Updated: 2024/04/27 22:36:48 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
