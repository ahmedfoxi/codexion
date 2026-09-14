# include <stdio.h>
# include <pthread.h>


typedef struct s_data
{
    int count;
    pthread_mutex_t mutex;
}   t_data;

void    *task(void *arg)
{
    t_data *a;
    int i = 0;


    a = (t_data *)arg;
    pthread_mutex_lock(&a->mutex);
    while (i < 1000000)
    {
        a->count++;
        i++;
    }
    
    printf("data: %d\n", a->count);
    pthread_mutex_unlock(&a->mutex);
    return NULL;
}

int main()
{
    pthread_t   t1;
    pthread_t   t2;
    t_data shared_data;


    shared_data.count = 0;
        
    pthread_mutex_init(&shared_data.mutex, NULL);
    
    pthread_create(&t1, NULL, task, &shared_data);
    pthread_create(&t2, NULL, task, &shared_data);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    pthread_mutex_destroy(&shared_data.mutex);
}
