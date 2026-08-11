#ifndef EX_H
#define EX_H


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>

typedef struct s_dongle
{
    int account;
    pthread_mutex_t mutex;
}t_dongle;

typedef struct s_dg
{
    pthread_t t1;
    pthread_t t2;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int ready;
}t_dg;


#endif
