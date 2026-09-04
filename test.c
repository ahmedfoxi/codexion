# include <stdio.h>
# include <pthread.h>


void    *task(void *data)
{
    int *b;
    int i = 0;


    b = (int *)data;
    while (i < 100)
    {
        i++;
        (*b)++;
    }
    printf("t :%d\n", *b);
    return (NULL);
}

int     main()
{
    pthread_t t1;
    pthread_t t2;
    int i = 0;


    pthread_create(&t1, NULL, task, &i);
    pthread_create(&t2, NULL, task, &i);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("n: %d\n", i);
}
