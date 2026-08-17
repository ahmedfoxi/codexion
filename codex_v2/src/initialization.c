# include "codex.h"


void ft_coders_init(t_data *data)//possible add pointer function and arg for pthread_create().
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

void ft_dongle_init(t_data *data)
{
    int i;
    int n_coders;


    i = 0;
    n_coders = data->number_of_coders;
    // free
    data->dongles = malloc(sizeof(t_dongle) * n_coders);
    if (!data->dongles)
        return ;

    while (i < n_coders)
    {
        data->dongles[i].id = i + 1;
        data->dongles[i].available = 1;
        data->dongles[i].available_at = 0;

        pthread_mutex_init(&data->dongles[i].mutex, NULL);
        pthread_cond_init(&data->dongles[i].cond, NULL);
        i++;
    }
}

void *coder_routing(void *arg)
{
    t_coder *coder;

    coder = (t_data *)arg;
    printf("%d %d has taken a dongle", coder->compile_count, coder->id);

    return NULL;
}

void create_threads(t_data *data)
{
    int i;


    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_create(
            &data->coders[i].thread,
            NULL,
            coder_routing,
            &data->coders[i]
        );
        i++;
    }
    i = 0;
    while (i < data->number_of_coders)
        pthread_join(data->coders[i++].thread, NULL);
}
