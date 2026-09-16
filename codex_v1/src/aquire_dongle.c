/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aquire_dongle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:02:41 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/15 23:58:43 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	wait_state_timeout(t_data *data, long wait_ms)
{
	pthread_mutex_unlock(&data->state_mutex);
	usleep(wait_ms * 1000);
	pthread_mutex_lock(&data->state_mutex);
}

int	cooldown_ok(t_dongle *dongle, t_data *data)
{
	if (dongle->last_release == 0)
		return (1);
	if (get_time_ms() - dongle->last_release >= data->dongle_cooldown)
		return (1);
	return (0);
}

int	can_take_two(t_coder *coder, t_data *data)
{
	t_request	*left_top;
	t_request	*right_top;

	if (coder->left->queue.size == 0
		|| coder->right->queue.size == 0)
		return (0);
	left_top = pick_next(coder->left);
	right_top = pick_next(coder->right);
	if (left_top->coder_id != coder->id
		|| right_top->coder_id != coder->id)
		return (0);
	if (!coder->left->available || !coder->right->available)
		return (0);
	if (!cooldown_ok(coder->left, data)
		|| !cooldown_ok(coder->right, data))
		return (0);
	return (1);
}

void	acquire_dongle(t_coder *coder, t_data *data)
{
	t_request	req;
	long		wait_ms;

	req = create_request(coder, data);
	pthread_mutex_lock(&data->state_mutex);
	heap_push(&coder->left->queue, req, data->scheduler);
	heap_push(&coder->right->queue, req, data->scheduler);
	while (is_running(data) && !can_take_two(coder, data))
	{
		wait_ms = get_cooldown_wait(coder, data);
		if (wait_ms > 0)
			wait_state_timeout(data, wait_ms);
		else
			pthread_cond_wait(&data->state_cond, &data->state_mutex);
	}
	if (is_running(data))
	{
		heap_pop(&coder->left->queue, data->scheduler);
		heap_pop(&coder->right->queue, data->scheduler);
		coder->left->available = 0;
		coder->right->available = 0;
	}
	pthread_mutex_unlock(&data->state_mutex);
}

void	take_dongles(t_coder *coder, t_data *data)
{
	acquire_dongle(coder, data);
	log_action(data, coder->id, "has taken a dongle");
	log_action(data, coder->id, "has taken a dongle");
}
