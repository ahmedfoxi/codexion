/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 17:17:56 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/08 20:41:47 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void    destroy_mutex(t_data    *data)
{
    int i;


    i = 0;
    while (i < data->number_of_coders)
        pthread_mutex_destroy(&data->coders[i++].coder_mutex);
    
    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_mutex_destroy(&data->dongles[i].mutex);
        pthread_cond_destroy(&data->dongles[i].cond);
        i++;
    }
}

void    clean_up(t_data *data)
{
    int i;


    i = 0;
    while (i < data->number_of_coders)
    {
        if (data->dongles[i].queue.requests)
            free(data->dongles[i].queue.requests);
        i++;
    }
    
    pthread_mutex_destroy(&data->log_mutex);
    pthread_mutex_destroy(&data->simulation_mutex);
    pthread_mutex_destroy(&data->counter_mutex);
    
    destroy_mutex(data);

    free(data->dongles);
    free(data->coders);
    free(data);
}

