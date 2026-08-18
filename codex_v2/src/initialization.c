# include "codex.h"


void coder_init(t_data *data)//possible add pointer function and arg for pthread_create().
{
    int i;


    i = 0;
    // free
    data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
    if (!data->coders)
        return ;

    while (i < data->number_of_coders)
    {
        data->coders[i].id = i + 1;
        data->coders[i].last_compile = 0;
        data->coders[i].compile_count = 0;
        i++;
    }
}

void dongle_init(t_data *data)
{
    int i;
    int n_coders;


    i = 0;
    n_coders = data->number_of_coders;
    data->dongles = malloc(sizeof(t_dongle) * n_coders);
    if (!data->dongles)
        return ;
    while (i < n_coders)
    {
        data->dongles[i].id = i + 1;
        data->dongles[i].available = 1;
        data->dongles[i].available_at = 0;

        data->dongles[i].queue.capacity = 0;
        data->dongles[i].queue.size = 2;
        data->dongles[i].queue.requests = malloc(sizeof(t_request) * 2);
        if (!data->dongles[i].queue.requests)
            return ;
        pthread_mutex_init(&data->dongles[i].mutex, NULL);
        pthread_cond_init(&data->dongles[i].cond, NULL);
        i++;
    }
}

void affiche(t_data *data)
{
    int     i=0;


    while (i < data->number_of_coders)
    {
        printf("%d\n", data->dongles[i].queue.size);
        i++;
    }
}
