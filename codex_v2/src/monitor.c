/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 18:36:41 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/07 23:09:47 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void    handle_bournout(t_data   *data, int  idx, long   now)
{
    pthread_mutex_lock(&data->log_mutex);
    printf("%ld %d burned out\n",
        now - data->start_time, data->coders[idx].id);
    pthread_mutex_lock(&data->simulation_mutex);
    data->runing = 0;
    pthread_mutex_unlock(&data->simulation_mutex);
    pthread_mutex_unlock(&data->log_mutex);
    wake_all(data);
}

void check_all_done(t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_mutex_lock(&data->coders[i].coder_mutex);
        if (data->coders[i].compile_count < data->number_of_compiles_required)
        {
            pthread_mutex_unlock(&data->coders[i].coder_mutex);
            return ;
        }
        pthread_mutex_unlock(&data->coders[i].coder_mutex);
        i++;
    }
    pthread_mutex_lock(&data->simulation_mutex);
    data->runing = 0;
    pthread_mutex_unlock(&data->simulation_mutex);
    wake_all(data);
}

void    check_burnout(t_data *data)
{
    long    now;
    int i;


    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_mutex_lock(&data->coders[i].coder_mutex);
        if (data->coders[i].compile_count >= data->number_of_compiles_required)
        {
            pthread_mutex_unlock(&data->coders[i].coder_mutex);
            continue;
        }
        now = get_time_ms();
        if (now - data->coders[i].last_compile >= data->time_to_burnout)
        {
            pthread_mutex_unlock(&data->coders[i].coder_mutex);
            handle_bournout(data, data->coders[i].id, now);
            continue;
        }
        pthread_mutex_unlock(&data->coders[i].coder_mutex);
        i++;
    }
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (is_running(data))
	{
		check_burnout(data);
		if (!is_running(data))
			break ;
		check_all_done(data);
		usleep(1000);
	}
	return (NULL);
}

