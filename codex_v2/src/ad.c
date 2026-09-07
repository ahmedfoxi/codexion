/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ad.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 20:29:46 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/07 17:48:27 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->last_release = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder->left);
	release_dongle(coder->right);
}

void	get_dongle_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_idx <= coder->right_idx)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}

int	cooldown_ok(t_dongle *dongle, t_data *data)
{
	if (dongle->last_release == 0)
		return (1);
	if (get_time_ms() - dongle->last_release >= data->dongle_cooldown)
		return (1);
	return (0);
}

static void	wait_cooldown(t_dongle *dongle, t_data *data)
{
	struct timeval	tv;
	struct timespec	ts;
	unsigned long	elapsed;
	unsigned long	remaining;

	elapsed = get_time_ms() - dongle->last_release;
	if (elapsed >= data->dongle_cooldown)
		return ;
	remaining = data->dongle_cooldown - elapsed;
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (tv.tv_usec / 1000 + remaining) / 1000;
	ts.tv_nsec = ((tv.tv_usec / 1000 + remaining) % 1000) * 1000000;
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}

// static int	can_take(t_dongle *dongle, unsigned int coder_id)
// {
// 	t_request	top;

// 	if (dongle->queue.size == 0)
// 		return (0);
// 	top = queue_peek(&dongle->queue);
// 	if (top.coder_id != coder_id)
// 		return (0);
// 	if (!dongle->available)
// 		return (0);
// 	return (1);
// }

static void	acquire_dongle(t_coder *coder, t_data *data, t_dongle *dongle)
{
	t_request	req;

	req.coder_id = coder->id;
	req.deadline = coder->last_compile + data->time_to_burnout;
	pthread_mutex_lock(&data->counter_mutex);
	req.arrival_order = data->request_counter++;
	pthread_mutex_unlock(&data->counter_mutex);

    pthread_mutex_lock(&dongle->mutex);

	queue_push(&dongle->queue, req);

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
		queue_pop(&dongle->queue);
		dongle->available = 0;
	}
	pthread_mutex_unlock(&dongle->mutex);
}

void	acquire_dongles(t_coder *coder, t_data *data)
{
	t_dongle	*first;
	t_dongle	*second;

	get_dongle_order(coder, &first, &second);
	acquire_dongle(coder, data, first);
	if (!is_running(data))
		return ;
	log_action(data, coder->id, "has taken a dongle");
	acquire_dongle(coder, data, second);
	if (!is_running(data))
		return ;
	log_action(data, coder->id, "has taken a dongle");
}
