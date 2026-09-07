/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_system.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 17:55:31 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/06 15:33:10 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


// for fifo = 0 the priority for arrival_time .
// for edf = 1 the priority for deadline .
t_request    request_dongle(t_data   *data, t_coder  *coder)
{
    t_request   request;


    request.coder_id = coder->id;
    request.arrival_order = elapsed_ms(data);
    request.deadline = coder->last_compile + data->time_to_burnout;

    return (request);
}

void    request(t_data *data)
{
    t_dongle *left;
    t_dongle *right;
    int     i;


    i = 0;
    while (i < data->number_of_coders)
    {
        left = data->coders[i].left;
        right = data->coders[i].right;
        heap_init(&left->queue, 2);
        heap_init(&right->queue, 2);
        heap_push(&left->queue, request_dongle(data, &data->coders[i]), data->scheduler);
        heap_push(&right->queue, request_dongle(data, &data->coders[i]), data->scheduler);
        i++;
    }   
}
