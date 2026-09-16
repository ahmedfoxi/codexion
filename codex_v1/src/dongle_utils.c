/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:07:33 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/15 15:18:39 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	release_dongles(t_coder *coder, t_data *data)
{
	long	now;

	pthread_mutex_lock(&data->state_mutex);
	now = get_time_ms();
	coder->left->available = 1;
	coder->left->last_release = now;
	coder->right->available = 1;
	coder->right->last_release = now;
	pthread_cond_broadcast(&data->state_cond);
	pthread_mutex_unlock(&data->state_mutex);
}

int	heap_init(t_heap *heap, int capacity)
{
	heap->capacity = capacity;
	heap->size = 0;
	heap->requests = malloc(sizeof(t_request) * capacity);
	if (!heap->requests)
		return (0);
	return (1);
}

static long	cooldown_remaining(t_dongle *dongle, t_data *data)
{
	long	elapsed;

	if (dongle->last_release == 0)
		return (0);
	elapsed = get_time_ms() - dongle->last_release;
	if (elapsed >= data->dongle_cooldown)
		return (0);
	return (data->dongle_cooldown - elapsed);
}

long	get_cooldown_wait(t_coder *coder, t_data *data)
{
	long	left;
	long	right;

	left = cooldown_remaining(coder->left, data);
	right = cooldown_remaining(coder->right, data);
	if (left > right)
		return (left);
	return (right);
}

t_request	*pick_next(t_dongle *d)
{
	if (d->queue.size == 0)
		return (NULL);
	return (&d->queue.requests[0]);
}
