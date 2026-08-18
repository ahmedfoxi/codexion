/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_system.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 17:55:31 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/18 20:49:55 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void    *request(void *data)
{
    t_data  *data_m;
    int     i;


    data_m = (t_data *)data;
    i = 0;
    pthread_mutex_lock(&data_m->dongles[i].mutex);
    while (i < data_m->number_of_coders)
    {
        // data_m->dongles[i].queue.requests.;
        i++;
    }
    pthread_mutex_unlock(&data_m->dongles[i].mutex);

    return NULL;
}

void    request_system(t_data *data)
{
    int     i;


    i = 0;
    while (i < data->number_of_coders)
        pthread_create(&data->coders[i++].thread, NULL, request, NULL);

    i = 0;
    while (i < data->number_of_coders)
        pthread_join(data->coders[i++].thread, NULL);

}
