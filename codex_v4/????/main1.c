/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 18:20:57 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/17 18:21:16 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


int	init_simulation(t_data *data, int ac, char **av)
{
	int	i;

	if (ac != 9)
		return (0);
	memset(data, 0, sizeof(t_data));
	data->number_of_coders = atoi(av[1]);
	data->time_to_burnout = atoi(av[2]);
	data->time_to_compile = atoi(av[3]);
	data->time_to_debug = atoi(av[4]);
	data->time_to_refactor = atoi(av[5]);
	data->number_of_compiles_required = atoi(av[6]);
	data->dongle_cooldown = atoi(av[7]);
	data->scheduler = (strcmp(av[8], "edf") == 0); /* 0 for fifo, 1 for edf */

	if (data->number_of_coders <= 0 || data->time_to_burnout <= 0)
		return (0);

	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);

	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->dongles || !data->coders)
		return (0);

	i = 0;
	while (i < data->number_of_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].available = 1;
		data->dongles[i].available_at = 0;
		pthread_mutex_init(&data->dongles[i].mutex, NULL);
		pthread_cond_init(&data->dongles[i].cond, NULL);
		heap_init(&data->dongles[i].queue, data->number_of_coders);

		data->coders[i].id = i + 1;
		data->coders[i].data = data;
		data->coders[i].compile_count = 0;
		data->coders[i].last_compile = get_time_ms();
		pthread_mutex_init(&data->coders[i].state_mutex, NULL);

		data->coders[i].left = &data->dongles[i];
		data->coders[i].right = &data->dongles[(i + 1) % data->number_of_coders];
		i++;
	}
	return (1);
}

void	clean_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		pthread_cond_destroy(&data->dongles[i].cond);
		pthread_mutex_destroy(&data->coders[i].state_mutex);
		free(data->dongles[i].queue.requests);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	free(data->dongles);
	free(data->coders);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	if (!init_simulation(&data, ac, av))
		return (1);

	i = 0;
	while (i < data.number_of_coders)
	{
		pthread_create(&data.coders[i].thread, NULL, coder_routine, &data.coders[i]);
		i++;
	}
	pthread_create(&data.monitor, NULL, monitor_routine, &data);

	pthread_join(data.monitor, NULL);
	i = 0;
	while (i < data.number_of_coders)
	{
		pthread_join(data.coders[i].thread, NULL);
		i++;
	}
	clean_simulation(&data);
	return (0);
}