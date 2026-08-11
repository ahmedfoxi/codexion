# include "ex.h"


void    *task(void *arg)
{
    int *i = malloc(sizeof(int));

    *i = 10 + 20;

    return (i);
}

int main()
{
    pthread_t t;
    void *result;


    pthread_create(&t, NULL, task, NULL);
    pthread_join(t, &result);

    printf("result: %d", *(int *)result);
    free(result);
}
