# include "ex.h"


void    *worker(void *arg)
{
    t_data *d1;


    d1 = (t_data *)arg;
    
    printf("==== workers ====\n");
    printf("id: %d\n", d1->id);
    printf("value: %d\n", d1->value);

    return (NULL);
}

int main()
{
    t_data data;
    pthread_t t1;

    data.id = 42;
    data.value = 199;


    printf("main\n");
    pthread_create(&t1, NULL, worker, &data);
    pthread_join(t1, NULL);


}
