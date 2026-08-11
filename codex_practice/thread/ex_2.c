#include "ex.h"


void    *worker(void    *arg)
{
    int *number = (int *)arg;

    printf("work: %d\n", *number);
    return (NULL);
}

// int main()
// {
//     pthread_t   t1;
//     pthread_t   t2;

//     printf("main\n");

//     pthread_create(&t1,  NULL, worker, NULL);
//     pthread_create(&t2,  NULL, worker, NULL);

//     pthread_join(t1, NULL);
//     pthread_join(t2, NULL);
// }

int main()
{
    pthread_t   t1;
    pthread_t   t2;
    int number = 2;
    int number1 = 1;

    printf("main\n");

    pthread_create(&t1, NULL, worker, &number);
    pthread_join(t1, NULL);

    pthread_create(&t2, NULL, worker, &number);
    pthread_join(t2, NULL);
}
