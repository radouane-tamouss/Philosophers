/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 18:18:56 by rtamouss          #+#    #+#             */
/*   Updated: 2024/04/29 17:58:37 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (initialize_philos_and_start(philos, &data, num_philos) == 1)
		return (0);
	monitor(philos);
	ft_free_and_join(&data, philos);
	return (0);
}
