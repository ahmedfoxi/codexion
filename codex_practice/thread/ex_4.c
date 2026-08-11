# include "ex.h"


void    *worker(void *arg)
{
    t_data *tmp;

    tmp = (t_data *)arg;
    printf("id: %d\n", tmp->id);
    return NULL;
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    t_data d1;
    t_data d2;
    t_data d3;

    d1.id = 1;
    d1.value = 201;
    d2.id = 2;
    d2.value = 202;
    d3.id = 3;
    d3.value = 203;

    printf("==== main ====\n");

    pthread_create(&t1, NULL, worker, &d1);
    pthread_create(&t2, NULL, worker, &d2);
    pthread_create(&t3, NULL, worker, &d3);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
}
