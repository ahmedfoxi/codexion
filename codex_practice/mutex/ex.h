#ifndef EX_H
#define EX_H

#include <pthread.h>

typedef struct s_data
{
    int account_1;
    int account_2;
    pthread_mutex_t mutex_1; 
    pthread_mutex_t mutex_2; 
}t_data;

// typedef struct s_philo
// {
//     int id;
//     // long last_meal;
//     // int meals;
//     pthread_mutex_t *left_fork;
//     pthread_mutex_t *right_fork;
// } t_philo;

// typedef struct s_philo
// {
//     int id;
//     long last_meal;
//     int meals;
//     pthread_mutex_t *left_fork;
//     pthread_mutex_t *right_fork;
// } t_philo;

// typedef struct s_table
// {
//     int number_of_philosophers;
//     long time_to_die;
//     long time_to_eat;
//     long time_to_sleep;

//     pthread_mutex_t forks[5];

//     t_philo philosophers[5];
// } t_table;

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif
