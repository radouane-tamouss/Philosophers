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
	int 	i;
	i = 1;
	if (ft_atoi(av[i]) > 200)
		return (0);
	while(i < ac)
	{
		if(ft_check_if_string(av[i]) == 0)
			return(printf("contains other chars , no numbers\n"), 0);
		if(ft_atoi(av[i]) < 0)
			return (printf("atoi failed less than 0\n"), 0);
		if (ft_atoi(av[1]) > 200)
			return (printf("try to give a number of philos less than 200\n"), 0);
		i++;
	}
    if (ft_atoi(av[1]) < 1)
    {
        return (printf("must be at least one philosopher\n"), 0);
    }
	return (1);
}

void *ft_thinking(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	printf("%d is thinking\n",philo->id);
	return NULL;
}

void *ft_sleeping(t_philo *philo)
{
	printf("%s%d is sleeping\n%s",WHITE,philo->id, DEFAULT);
	ft_usleep(philo->data->time_to_sleep);
	return NULL;
}

void ft_print(t_philo *philo, char *str, char *color)
{
	pthread_mutex_lock(philo->data->print);
	// (void) color;
	printf("%s%ld %d %s\n%s",color,get_current_time() - philo->start, philo->id + 1, str, DEFAULT);
	// printf("%ld %d %s\n",get_current_time() - philo->start, philo->id + 1, str);
	pthread_mutex_unlock(philo->data->print);
}

int monitor(t_philo *philos)
{
	while(1)
	{
		int i = 0;
		while(i < philos->data->num_of_philos)
		{
			pthread_mutex_lock(philos[i].data->last_meal_mutex);
			if ((get_current_time() - philos[i].last_meal > philos[i].data->time_to_die) && philos[i].eating == 0)
			{
				printf("id of philo is %d\nlast meal time = %zu\ncurrent time : %zu\ntime - last meal : %zu <> %zu\n",philos[i].id + 1, philos[i].last_meal - philos[i].start,get_current_time() - philos[i].start, get_current_time() - philos[i].start - philos[i].last_meal, (size_t)philos[i].data->time_to_die);
				printf("id of philo is : %d\n", philos[i].id + 1);
				printf("philos last meal = %zu\n", philos[i].last_meal);
				printf("current time : %zu\n", get_current_time());
				printf("time - last meal : %zu <> %zu\n", get_current_time() - philos[i].last_meal, (size_t)philos[i].data->time_to_die);
				ft_print(&philos[i], "died", RED);
				*(philos[i].data->dead) = 1;
				pthread_mutex_unlock(philos[i].data->last_meal_mutex);
				return (1);
			}
			pthread_mutex_unlock(philos[i].data->last_meal_mutex);
			i++;
		}
		
		//if (philo->num_times_to_eat != 0 && philo->nb_meals_eating >= philo->num_times_to_eat)
		//{
		//	ft_print(philo, "has eaten enough", YELLOW);
		//	return (1);
		//}
	}
	return (0);
}
void *routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	while(1)
	{

		if (*philo->data->dead == 1)
			return NULL;
		if (philo->data->num_of_philos % 2 == 1)
		{
			if (philo->id % 2 == 1)
				ft_usleep(50);
		}
		pthread_mutex_lock(philo->l_fork);
		ft_print(philo, "has taken a fork", YELLOW);
		pthread_mutex_lock(philo->r_fork);
		ft_print(philo, "has taken a fork", YELLOW);
		
		ft_print(philo, "is eating", GREEN);
		pthread_mutex_lock(philo->data->last_meal_mutex);
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(philo->data->last_meal_mutex);
		philo->eating = 1;
		ft_usleep(philo->data->time_to_eat);
		philo->nb_meals_eaten++;

		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		philo->eating = 0;
		ft_print(philo, "is sleeping", WHITE);
		ft_usleep(philo->data->time_to_sleep);
		ft_print(philo, "is thinking", CYAN);
		usleep(500);
	}
	return NULL;
}
int main(int ac , char **av)
{
  if(ac != 5 && ac != 6)
    return (0);
 	if(ft_parsing(ac, av) == 0)
 	{
		printf("Error try to provide valid args!\n");	
		return (0);
 	}
	if(av[5] != NULL)
		printf("numer of times each philosopher must eat: %d\n", ft_atoi(av[5]));
    t_philo *philos = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	t_data data;
	data.num_of_philos = ft_atoi(av[1]);
	data.time_to_die = ft_atoi(av[2]);
	data.time_to_eat = ft_atoi(av[3]);
	data.time_to_sleep = ft_atoi(av[4]);
	// data.start = get_current_time();
	if(av[5] != NULL)
		data.num_times_to_eat = ft_atoi(av[5]);
	data.dead = malloc(sizeof(int));
	*data.dead = 0;
	data.print = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(data.print, NULL) != 0)
	{
		printf("mutex_init failed\n");
		return 0;
	}
	data.last_meal_mutex = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(data.last_meal_mutex, NULL) != 0)
	{
		printf("mutex_init failed\n");
		return 0;
	}
	
    int i = 0;
	printf("i = %d\n", i);
	i = 0;
	printf("number of forks %d\n", ft_atoi(av[1]));
    pthread_mutex_t *forks;
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
	i = 0;
	while(i < ft_atoi(av[1]))
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			printf("mutex_init failed\n");
			return 0;
		}
		// printf("forks[%d] = %p\n", i, &forks[i]);
		i++;
	}
	data.forks = forks;

	i = 0;
	while(i < ft_atoi(av[1]))
	{
		philos[i].id = i;
		// philos[i].time_to_die = ft_atoi(av[2]);
		// philos[i].time_to_eat = ft_atoi(av[3]);
		// philos[i].time_to_sleep = ft_atoi(av[4]);
		philos[i].last_meal = get_current_time();
		philos[i].eating = 0;
		philos[i].data = &data;
		philos[i].nb_meals_eaten = 0;
		// philos[i].num_of_philos = ft_atoi(av[1]);
		// philos[i].dead = malloc(sizeof(int));
		// *philos[i].dead = 0;
		if(philos[i].id % 2)
		{
			philos[i].l_fork = &forks[i];
			philos[i].r_fork = &forks[(i + 1) % ft_atoi(av[1])];
		}
		else
		{
			philos[i].r_fork = &forks[i];
			philos[i].l_fork = &forks[(i + 1) % ft_atoi(av[1])];
		}
		// philos[i].print = malloc(sizeof(pthread_mutex_t));
		// philos[i].last_meal_mutex = malloc(sizeof(pthread_mutex_t));
		// if (pthread_mutex_init(philos[i].print, NULL) != 0)
		// {
		// 	printf("mutex_init failed\n");
		// 	return 0;
		// }
		// if (pthread_mutex_init(philos[i].last_meal_mutex, NULL) != 0)
		// {
		// 	printf("mutex_init failed\n");
		// 	return 0;
		// }
		// if(av[5] != NULL)
		// 	philos[i].num_times_to_eat= ft_atoi(av[5]);
		philos[i].start = get_current_time();
		if(*philos->data->dead == 1)
			break;
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			return (printf("pthread_create failed\n"), 0);
		i++;	
	}
	// if (monitor(philos) == 1)
	// 	return (0);
	// -------------------------------------------------------------------
	// ------------------------------------------------
	// i = 1;
	// pthread_t *monitor = malloc(sizeof(pthread_t));
	// if (pthread_create(monitor, NULL, &routine, &philos[i]) != 0)
	// 	return (printf("pthread_create failed\n"), 0);

	i = 0;
	while(i < ft_atoi(av[1]))
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (printf("pthread_join failed\n"), 0);
		// printf("philosopher %d joined\n", i);
		i++;
	}
	// if (pthread_join(*monitor, NULL) != 0)
	// 	return (printf("pthread_join failed\n"), 0);
	// printf("monitor joined\n");	
  return (0);
}
