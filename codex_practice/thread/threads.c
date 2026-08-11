#include "ex.h"


void    *ft_hello(void *arg)
{
    printf("hello\n");
    return NULL;
}

void    *ft_print(void *arg)
{
    printf("print\n");
    return NULL;
}

int main()
{
    pthread_t   t1;
    pthread_t   t2;

    
    printf("main\n");

    pthread_create(&t2, NULL, ft_hello, NULL);
    pthread_join(t2, NULL);


    pthread_create(&t1, NULL, ft_print, NULL);
    pthread_join(t1, NULL);


}
