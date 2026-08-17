/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_calule.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 14:45:00 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/17 14:52:40 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


long    get_time_ms(void)
{
    struct timeval  tv;


    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void    precise_sleep(long ms, t_data *data)
{
    long    start;


    start = get_time_ms();
    while(get_time_ms() - start < ms)
    {
        pthread_mutex_lock(&data_>stop_mutex);
        if (data->stop)
        {
            pthread_mutex_unlock(&data_>stop_mutex);
            break;
        }
        pthread_mutex_unlock(&data_>stop_mutex);
        usleep(250);
    }
}
