#ifndef PHILO_H
#define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

// colors macros
#define RED "\033[0;31m"
#define WHITE "\033[0;37m"
#define DEFAULT "\033[0m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"



typedef struct s_data
{
    int     num_of_philos;
    size_t     time_to_die;
    int     time_to_eat;
    int     time_to_sleep;
    int     num_times_to_eat;
    int     *dead;
    size_t  start;
    pthread_mutex_t *forks;
    pthread_mutex_t print;
    pthread_mutex_t dead_lock;
    // pthread_mutex_t print;
    pthread_mutex_t last_meal_mutex;
} t_data;
typedef struct s_philo
{
    pthread_t thread;
    int     id;
    int     eating;
    int     nb_meals_eaten;
    size_t  last_meal;
    // size_t  time_to_eat;
    // size_t  time_to_die;
    // size_t  time_to_sleep;
    // size_t  start_time;
    // int     num_of_philos;
    // int     num_times_to_eat;
    // int     *dead;
	size_t start;
	// pthread_mutex_t *last_meal_mutex;
    pthread_mutex_t r_fork;
    pthread_mutex_t l_fork;
    t_data  *data;
    // pthread_mutex_t *print;

} t_philo;


#endif
