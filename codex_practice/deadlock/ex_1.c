#include "ex.h"


void *task(void *arg)
{
    t_dongle *tmp;

    tmp = (t_dongle *)arg;

    pthread_mutex_lock(&tmp->mutex);
    tmp->account += 10;
    pthread_mutex_unlock(&tmp->mutex);

    return (NULL);
}

int main()
{
    pthread_t codex_1;
    pthread_t codex_2;
    t_dongle dongle_1;
    t_dongle dongle_2;

    dongle_1.account = 100;
    dongle_2.account = 200;

    // mutex
    pthread_mutex_init(&dongle_1.mutex, NULL);
    pthread_mutex_init(&dongle_2.mutex, NULL);

    // thread
    pthread_create(&codex_1, NULL, task, &dongle_1);
    pthread_create(&codex_2, NULL, task, &dongle_2);

    pthread_join(codex_1, NULL);
    pthread_join(codex_2, NULL);


    pthread_mutex_destroy(&dongle_1.mutex);
    pthread_mutex_destroy(&dongle_2.mutex);


    printf("account_1: %d\n", dongle_1.account);
    printf("account_2: %d\n", dongle_2.account);
}
