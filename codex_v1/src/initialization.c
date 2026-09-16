/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 00:20:26 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/16 00:18:07 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static int	coder_init(t_data *data)
{
	int	i;

	i = 0;
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (0);
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].last_compile = 0;
		data->coders[i].compile_count = 0;
		data->coders[i].data = data;
		pthread_mutex_init(&data->coders[i].coder_mutex, NULL);
		i++;
	}
	return (1);
}

static int	dongle_init(t_data *data)
{
	int	i;
	int	n_coders;

	i = 0;
	n_coders = data->number_of_coders;
	data->dongles = malloc(sizeof(t_dongle) * n_coders);
	if (!data->dongles)
		return (0);
	while (i < n_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].available = 1;
		data->dongles[i].last_release = 0;
		if (!heap_init(&data->dongles[i].queue,
				data->number_of_compiles_required * 2))
			return (0);
		i++;
	}
	return (1);
}

static void	link_coder_dongle(t_data *data)
{
	int	i;
	int	n;

	n = data->number_of_coders;
	i = 0;
	while (i < n)
	{
		data->coders[i].left = &data->dongles[(i - 1 + n) % n];
		data->coders[i].right = &data->dongles[i];
		i++;
	}
}

// static void	ft_indexing(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->number_of_coders)
// 	{
// 		data->coders[i].left_idx = data->coders[i].left->id;
// 		data->coders[i].right_idx = data->coders[i].right->id;
// 		i++;
// 	}
// }

// static void	affiche(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->number_of_coders)
// 	{
// 		printf("coder %d: left=dongle[%d] right=dongle[%d]\n",
// 		data->coders[i].id, data->coders[i].left->id,
// 		data->coders[i].right->id);
// 		i++;
// 	}
// }

int	init_data(t_data *data)
{
	if (!dongle_init(data))
		return (0);
	if (!coder_init(data))
		return (0);
	link_coder_dongle(data);
	// ft_indexing(data);
	pthread_mutex_init(&data->counter_mutex, NULL);
	pthread_mutex_init(&data->log_mutex, NULL);
	pthread_mutex_init(&data->simulation_mutex, NULL);
	pthread_mutex_init(&data->state_mutex, NULL);
	pthread_cond_init(&data->state_cond, NULL);
	return (1);
}
