/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:12:48 by rtamouss          #+#    #+#             */
/*   Updated: 2024/04/29 17:57:55 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(const char *str)
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
	if (ft_check_if_string(av[ac - 1]) == 0)
		return (printf("Error: Arguments must contain only digits.\n"), 0);
	if (ft_atoi(av[ac - 1]) == 0 && ac == 6)
		return (0);
	while (i < ac)
	{
		if (ft_check_if_string(av[i]) == 0)
			return (printf("Error: Arguments must contain only digits.\n"), 0);
		if (ft_atoi(av[i]) <= 0 || ft_atoi(av[i]) > 2147483647)
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
