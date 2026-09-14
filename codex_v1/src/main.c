/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:55:03 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/15 00:47:48 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	wake_all(t_data *data)
{
	pthread_mutex_lock(&data->state_mutex);
	pthread_cond_broadcast(&data->state_cond);
	pthread_mutex_unlock(&data->state_mutex);
}

static void	start_simulation(t_data *data)
{
	int	i;

	data->start_time = get_time_ms();
	i = 0;
	while (i < data->number_of_coders)
		data->coders[i++].last_compile = data->start_time;
	data->runing = 1;
	pthread_create(&data->monitor, NULL, monitor_routine, data);
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_create(&data->coders[i].thread, NULL,
			coder_routine, &data->coders[i]);
		i++;
	}
	pthread_join(data->monitor, NULL);
	wake_all(data);
	i = 0;
	while (i < data->number_of_coders)
		pthread_join(data->coders[i++].thread, NULL);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac != 9)
	{
		write(2, "ERROR", 5);
		return (0);
	}
	data = ft_parse(av);
	if (data->number_of_coders == 1)
	{
		printf("0 1 burned out\n");
		free(data);
		return (0);
	}
	init_data(data);
	start_simulation(data);
	clean_up(data);
	return (0);
}
