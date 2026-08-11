# include "ex.h"


void *task(void *arg)
{
    t_data *p_data;


    p_data = (t_data *)arg;

    pthread_mutex_lock(&p_data->mutex_1);
    pthread_mutex_lock(&p_data->mutex_2);
    p_data->account_1 += 20;
    
    p_data->account_2 += 40;
    pthread_mutex_unlock(&p_data->mutex_1);
    pthread_mutex_lock(&p_data->mutex_2);

    printf("account_1: %d\n", p_data->account_1);
    printf("account_2: %d\n", p_data->account_2);

    return (NULL);
}

int main()
{
    // this example demonstrat the Deadlock
    pthread_t coder_1;
    pthread_t coder_2;
    t_data dangle;

    dangle.account_1 = 100;
    dangle.account_2 = 200;

    pthread_mutex_init(&dangle.mutex_1, NULL);
    pthread_mutex_init(&dangle.mutex_2, NULL);


    pthread_create(&coder_1, NULL, task, &coder_1);
    pthread_create(&coder_2, NULL, task, &coder_2);

    pthread_join(coder_1, NULL);
    pthread_join(coder_2, NULL);


    pthread_mutex_destroy(&dangle.mutex_1);
    pthread_mutex_destroy(&dangle.mutex_2);

}

