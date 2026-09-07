/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_calule.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 14:45:00 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/07 22:45:25 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


long    get_time_ms(void)
{
    struct timeval  tv;


    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	ft_usleep(long ms, t_data *data)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
	{
		if (!is_running(data))
			break ;
		usleep(500);
	}
}

void    precise_sleep(long ms, t_data *data)
{
    long    start;


    start = get_time_ms();
    while(get_time_ms() - start < ms)
    {
        pthread_mutex_lock(&data->simulation_mutex);
        if (data->runing)
        {
            pthread_mutex_unlock(&data->simulation_mutex);
            break;
        }
        pthread_mutex_unlock(&data->simulation_mutex);
        usleep(250);
    }
}

long    elapsed_ms(t_data   *data)
{
    return (get_time_ms() - data->start_time);
}
