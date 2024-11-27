/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:16:50 by rtamouss          #+#    #+#             */
/*   Updated: 2024/04/27 22:36:13 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
