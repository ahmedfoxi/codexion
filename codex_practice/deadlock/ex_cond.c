#include "ex.h"


void *task_1(void *arg)
{
    t_dg *tmp;


    tmp = (t_dg *)arg;

    pthread_mutex_lock(&tmp->mutex);
    tmp->ready += 10;
    pthread_cond_signal(&tmp->cond);
    pthread_mutex_unlock(&tmp->mutex);

    printf("tread_1: %d\n", tmp->ready);

    return (NULL);
}

void *task_2(void *arg)
{
    t_dg *tmp;


    tmp = (t_dg *)arg;
    pthread_mutex_lock(&tmp->mutex);
    
    while (tmp->ready)
        pthread_cond_wait(&tmp->cond, &tmp->mutex);
    
    tmp->ready += 10;

    pthread_mutex_unlock(&tmp->mutex);

    printf("tread_2: %d\n", tmp->ready);

    return (NULL);
}

int main()
{
    t_dg data;


    data.ready = 0;

    pthread_cond_init(&data.cond, NULL);
    pthread_mutex_init(&data.mutex, NULL);

    pthread_create(&data.t1, NULL, task_1, &data);
    pthread_create(&data.t2, NULL, task_2, &data);

    pthread_join(data.t1, NULL);
    pthread_join(data.t2, NULL);

    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond);

    printf("value: %d\n", data.ready);

}
