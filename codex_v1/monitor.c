/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:09:45 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/17 17:17:23 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


static void	wake_all_threads(t_data *data)
{
	int	i;

    
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

static int	check_coder_burnout(t_coder *coder, t_data *data)
{
	long	now;
	long	elapsed;

	pthread_mutex_lock(&coder->state_mutex);
	now = get_time_ms();
	elapsed = now - coder->last_compile;
	if (elapsed >= data->time_to_burnout)
	{
		pthread_mutex_unlock(&coder->state_mutex);
		pthread_mutex_lock(&data->print_mutex);
		pthread_mutex_lock(&data->stop_mutex);
		if (!data->stop)
		{
			data->stop = 1;
			printf("%ld %d burned out\n", now, coder->id);
		}
		pthread_mutex_unlock(&data->stop_mutex);
		pthread_mutex_unlock(&data->print_mutex);
		wake_all_threads(data);
		return (1);
	}
	pthread_mutex_unlock(&coder->state_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	int		all_completed;

	data = (t_data *)arg;
	while (1)
	{
		all_completed = (data->number_of_compiles_required > 0);
		i = 0;
		while (i < data->number_of_coders)
		{
			if (check_coder_burnout(&data->coders[i], data))
				return (NULL);

			pthread_mutex_lock(&data->coders[i].state_mutex);
			if (data->number_of_compiles_required > 0 &&
				data->coders[i].compile_count < data->number_of_compiles_required)
				all_completed = 0;
			pthread_mutex_unlock(&data->coders[i].state_mutex);
			i++;
		}
		if (all_completed)
		{
			pthread_mutex_lock(&data->stop_mutex);
			data->stop = 1;
			pthread_mutex_unlock(&data->stop_mutex);
			wake_all_threads(data);
			return (NULL);
		}
		usleep(500); /* 0.5 ms cycle to guarantee <= 10 ms burnout precision */
	}
	return (NULL);
}
