/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aquire_dongle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:02:41 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/07 22:57:42 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


int	cooldown_ok(t_dongle *dongle, t_data *data)
{
	if (dongle->last_release == 0)
		return (1);
	if (get_time_ms() - dongle->last_release >= data->dongle_cooldown)
		return (1);
	return (0);
}

void	wait_cooldown(t_dongle *dongle, t_data *data)
{
	struct timeval	tv;
	struct timespec	ts;
	long	elapsed;
	long	remaining;

	elapsed = get_time_ms() - dongle->last_release;
	if (elapsed >= data->dongle_cooldown)
		return ;
	remaining = data->dongle_cooldown - elapsed;
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (tv.tv_usec / 1000 + remaining) / 1000;
	ts.tv_nsec = ((tv.tv_usec / 1000 + remaining) % 1000) * 1000000;
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}

int can_take(t_dongle *dongle, int coder_id)
{
    t_request *top;


    if (dongle->queue.size == 0)
        return (0);
    top = pick_next(dongle);
	if (top->coder_id != coder_id)
		return (0);
	if (!dongle->available)
		return (0);
	return (1);
}

void	acquire_dongle(t_coder *coder, t_data *data, t_dongle *dongle)
{
	t_request	req;

	req.coder_id = coder->id;
	req.deadline = coder->last_compile + data->time_to_burnout;
	pthread_mutex_lock(&data->counter_mutex);
	req.arrival_order = data->request_counter++;
	pthread_mutex_unlock(&data->counter_mutex);

    pthread_mutex_lock(&dongle->mutex);

	heap_push(&dongle->queue, req, data->scheduler);

    while (is_running(data)
		&& (!can_take(dongle, coder->id) || !cooldown_ok(dongle, data)))
	{
		if (can_take(dongle, coder->id))
			wait_cooldown(dongle, data);
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (is_running(data))
	{
		heap_pop(&dongle->queue, data->scheduler);
		dongle->available = 0;
	}
	pthread_mutex_unlock(&dongle->mutex);
}

void	take_dongles(t_coder *coder, t_data *data)
{
	t_dongle	*first;
	t_dongle	*second;

	get_dongle_order(coder, &first, &second);
	acquire_dongle(coder, data, first);
	if (!is_running(data))
		return ;
	log_action(data, "has taken a dongle");
	acquire_dongle(coder, data, second);
	if (!is_running(data))
		return ;
	log_action(data, "has taken a dongle");
}
