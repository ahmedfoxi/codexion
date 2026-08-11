# include "ex.h"

void    *task_1(void *n)
{
    int *ary;
    int i = 0;


    ary = (int *)n;
    while (i <= 24)
    {
        ary[i] = 1;
        i++;
    }
    return NULL;
}

void    *task_2(void *n)
{
    int *ary;
    int i = 25;


    ary = (int *)n;
    while (i <= 49)
    {
        ary[i] = 2;
        i++;
    }
    return NULL;
}

void    *task_3(void *n)
{
    int *ary;
    int i = 50;


    ary = (int *)n;
    while (i <= 74)
    {
        ary[i] = 3;
        i++;
    }
    return NULL;
}

void    *task_4(void *n)
{
    int *ary;
    int i = 75;


    ary = (int *)n;
    while (i <= 99)
    {
        ary[i] = 4;
        i++;
    }
    return NULL;
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    int numbers[100];
    int i = 0;


    pthread_create(&t1, NULL, task_1, &numbers);
    pthread_create(&t2, NULL, task_2, &numbers);
    pthread_create(&t3, NULL, task_3, &numbers);
    pthread_create(&t4, NULL, task_4, &numbers);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    while (i < 100)
    {
        printf("I: %d V: %d-", i, numbers[i]);
        i++;
    }
}
