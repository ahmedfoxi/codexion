/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_action.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 16:38:44 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/15 14:45:28 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	log_action(t_data *data, int id, char *msg)
{
	pthread_mutex_lock(&data->log_mutex);
	if (is_running(data))
		printf("%lld %d %s\n", get_time_ms() - data->start_time, id, msg);
	pthread_mutex_unlock(&data->log_mutex);
}

int	is_running(t_data *data)
{
	int	run;

	pthread_mutex_lock(&data->simulation_mutex);
	run = data->runing;
	pthread_mutex_unlock(&data->simulation_mutex);
	return (run);
}

t_request	create_request(t_coder	*coder, t_data	*data)
{
	t_request	req;

	req.coder_id = coder->id;
	pthread_mutex_lock(&coder->coder_mutex);
	req.deadline = coder->last_compile + data->time_to_burnout;
	pthread_mutex_unlock(&coder->coder_mutex);
	pthread_mutex_lock(&data->counter_mutex);
	req.arrival_order = data->request_counter++;
	pthread_mutex_unlock(&data->counter_mutex);
	return (req);
}
