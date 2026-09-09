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
        data->coders[i].data = data; // rev
        pthread_mutex_init(&data->coders[i].coder_mutex, NULL);
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

        heap_init(&data->dongles[i].queue, 2);

        pthread_mutex_init(&data->dongles[i].mutex, NULL);
        pthread_cond_init(&data->dongles[i].cond, NULL);
        i++;
    }
}

void link_coder_dongle(t_data *data)
{
    int i;
    int n;


    n = data->number_of_coders;
    i = 0;
    while (i < n)
    {
        data->coders[i].left = &data->dongles[(i - 1 + n) % n];
        data->coders[i].right = &data->dongles[i];
        i++;
    }
}

void affiche(t_data *data)
{
    int     i=0;


    while (i < data->number_of_coders)
    {
        printf("coder %d: left=dongle[%d] right=dongle[%d]\n",
            data->coders[i].id, data->coders[i].left->id, data->coders[i].right->id);
        i++;
    }
}

void    init_data(t_data *data)
{
    dongle_init(data);
    coder_init(data);
    link_coder_dongle(data);    
    
    pthread_mutex_init(&data->counter_mutex, NULL);
    pthread_mutex_init(&data->log_mutex, NULL);
    pthread_mutex_init(&data->counter_mutex, NULL);

}
