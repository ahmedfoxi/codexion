/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 18:32:00 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/07 22:28:45 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


int	is_running(t_data *data)
{
	int	running;

	pthread_mutex_lock(&data->simulation_mutex);
	running = data->running;
	pthread_mutex_unlock(&data->simulation_mutex);
	return (running);
}

void	wake_all(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

static void	start_simulation(t_data *data)
{
	unsigned int	i;

	data->start_time = get_time_ms();
	i = 0;
	while (i < data->number_of_coders)
		data->coders[i++].last_compile_ms = data->start_time;
	data->running = true;
	pthread_create(&data->monitor_thread, NULL, monitor_routine, data);
	i = 0;
	while (i++ < data->number_of_coders)
		pthread_create(&data->coders[i - 1].thread, NULL, coder_routine,
			&data->coders[i - 1]);
	pthread_join(data->monitor_thread, NULL);
	wake_all(data);
	i = 0;
	while (i < data->number_of_coders)
		pthread_join(data->coders[i++].thread, NULL);
}


int	main(int argc, char **argv)
{
	t_data	data;

	parse_args(argc, argv, &data);
	init_data(&data);
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
