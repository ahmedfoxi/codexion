# include "ex.h"


void    *task_1(void *n)
{
    int *ary;
    int i = 0;
    int *sum = malloc(sizeof(int));

    *sum = 0;
    ary = (int *)n;
    while (i < 250)
    {
        ary[i] = 1;
        *sum += ary[i];
        i++;
    }
    printf("Chunck sum (task_1): %d\n", *sum);
    return sum;
}

void    *task_2(void *n)
{
    int *ary;
    int i = 250;
    int *sum = malloc(sizeof(int));


    *sum = 0;
    ary = (int *)n;
    while (i < 500)
    {
        ary[i] = 2;
        *sum += ary[i];
        i++;
    }
    printf("Chunck sum (task_2): %d\n", *sum);
    return sum;
}

void    *task_3(void *n)
{
    int *ary;
    int i = 500;
    int *sum = malloc(sizeof(int));


    *sum = 0;
    ary = (int *)n;
    while (i < 750)
    {
        ary[i] = 3;
        *sum += ary[i];
        i++;
    }
    printf("Chunck sum (task_3): %d\n", *sum);
    return sum;
}

void    *task_4(void *n)
{
    int *ary;
    int i = 750;
    int *sum = malloc(sizeof(int));


    *sum = 0;
    ary = (int *)n;
    while (i < 1000)
    {
        ary[i] = 4;
        *sum += ary[i];
        i++;
    }
    printf("Chunck sum (task_4): %d\n", *sum);
    return sum;
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    int numbers[1000];
    void *r1;
    void *r2;
    void *r3;
    void *r4;
    int all_sum = 0;


    pthread_create(&t1, NULL, task_1, &numbers);
    pthread_create(&t2, NULL, task_2, &numbers);
    pthread_create(&t3, NULL, task_3, &numbers);
    pthread_create(&t4, NULL, task_4, &numbers);

    pthread_join(t1, &r1);
    pthread_join(t2, &r2);
    pthread_join(t3, &r3);
    pthread_join(t4, &r4);

    all_sum = *(int*)r1 + *(int*)r2 + *(int*)r3 + *(int*)r4;
    printf("all_sum: %d\n", all_sum);

    free(r1);
    free(r3);
    free(r2);
    free(r4);
}


// typedef struct s_task
// {
//     int *array;
//     int start;
//     int end;
// } t_task;

// For example:

// T1 → { numbers, 0,   250 }
// T2 → { numbers, 250, 500 }
// T3 → { numbers, 500, 750 }
// T4 → { numbers, 750, 1000 }
