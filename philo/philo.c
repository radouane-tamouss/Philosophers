#include "philo.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

size_t get_current_time(void)
{
	struct timeval time;
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

int ft_atoi(const char *str)
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

int ft_check_if_string(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if( (str[i] >= '0' && str[i] <= '9') || ((str[i] == '+' || str[i] == '-') && (str[i+1] >= '0'&& str[i+1] <= '9')))
			i++;
		else
			return (0);
	}
	return (1);
}

int ft_parsing(int ac, char **av)
{
	int i;
	i = 1;
	if (ft_atoi(av[i]) > 200)
		return (0);
	while(i < ac)
	{
		if (ft_check_if_string(av[i]) == 0)
			return (printf("contains non digit characters\n"), 0);
		if (ft_atoi(av[i]) < 0)
			return (printf("negative number\n"), 0);
		if (ft_atoi(av[i]) > 200)
			return (printf("number of philosophers is too high\n"), 0);
		i++;
	}
	if (ft_atoi(av[1]) < 1)
		return (printf("number of philosophers must be at least one\n"), 0);
	return (1);
}
void print_action(t_philo *philo, char *action, char *color)
{
	pthread_mutex_lock(philo->print);
	printf("%s%ld %d %s%s\n",color, get_current_time() - philo->start, philo->id, action, color);
	pthread_mutex_unlock(philo->print);
}
int monitor(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(philo->last_meal_mutex);
		if (get_current_time() - philo->last_meal > philo->time_to_die)
		{
			print_action(philo, "********died*******\n", RED);
			*philo->dead = 1;
			pthread_mutex_unlock(philo->last_meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(philo->last_meal_mutex);
		// if (philo->num_times_to_eat != -1 && philo->nb_meals_eating >= philo->num_times_to_eat)
		// {
		// 	*philo->dead = 1;
		// 	return (1);
		// }
	}
	return (0);
}
void *routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	while (1)
	{
		pthread_mutex_lock(philo->l_fork);
		print_action(philo, "has taken a fork", YELLOW);
		pthread_mutex_lock(philo->r_fork);
		print_action(philo, "has taken a fork", YELLOW);
		print_action(philo, "is eating", GREEN);
		philo->eating = 1;
		pthread_mutex_lock(philo->last_meal_mutex);
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(philo->last_meal_mutex);
		// philo->nb_meals_eating++;
		ft_usleep(philo->time_to_eat);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		philo->eating = 0;
		print_action(philo, "is sleeping", CYAN);
		ft_usleep(philo->time_to_sleep);
		print_action(philo, "is thinking", WHITE);
	}
	return (NULL);
}

int main(int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (0);
	if (ft_parsing(ac, av) == 0)
		return (printf("Error, Invalid arguments\n"), 0);
	
	t_philo *philos;
	philos = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	int i = 0;
	pthread_mutex_t *forks;
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
	while (i < ft_atoi(av[1]))
	{
		if(pthread_mutex_init(&forks[i], NULL) != 0)
			return (printf("mutex init failed\n"), 0);
		i++;
	}
	i = 0;
	while(i < ft_atoi(av[1]))
	{
		philos[i].id = i + 1;
		philos[i].time_to_die = ft_atoi(av[2]);
		philos[i].time_to_eat = ft_atoi(av[3]);
		philos[i].time_to_sleep = ft_atoi(av[4]);
		philos[i].last_meal = get_current_time();
		philos[i].eating = 0;
		philos[i].num_of_philos = ft_atoi(av[1]);
		philos[i].dead = malloc(sizeof(int));
		philos[i].nb_meals_eating = 0;
		*philos[i].dead = 0;
		if (philos[i].id % 2)
		{
			philos[i].l_fork = &forks[i];
			philos[i].r_fork = &forks[(i + 1) % ft_atoi(av[1])];
		}
		else
		{
			philos[i].r_fork = &forks[i];
			philos[i].l_fork = &forks[(i + 1) % ft_atoi(av[1])];
		}
		philos[i].print = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(philos[i].print, NULL) != 0)
			return (printf("mutex init failed\n"), 0);
		philos[i].last_meal_mutex = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(philos[i].last_meal_mutex, NULL) != 0)
			return (printf("mutex init failed\n"), 0);
		if (av[5] != NULL)
			philos[i].num_times_to_eat = ft_atoi(av[5]);
		else
			philos[i].num_times_to_eat = -1;
		philos[i].start = get_current_time();
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			return (printf("pthread_create failed\n"), 0);
		i++;
	}


	if (monitor(philos) == 1)
	{
		// i = 0;
		// while(i < ft_atoi(av[1]))
		// {
		// 	printf("start joining\n\n");
		// 	if (pthread_join(philos[i].thread, NULL) != 0)
		// 		return (printf("pthread_join failed\n"), 0);
		// 	i++;
		// }
		return (0);
	}
	i = 0;
	while(i < ft_atoi(av[1]))
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (printf("pthread_join failed\n"), 0);
		i++;
	}











	return (0);

}