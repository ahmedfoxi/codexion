/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aquire_dongle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:02:41 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/13 12:40:38 by ahbarbou         ###   ########.fr       */
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

int request_can_run(t_request *req, t_data *data)
{
    t_dongle *left;
    t_dongle *right;

    left = &data->dongles[req->left_dongle - 1];
    right = &data->dongles[req->right_dongle - 1];

    if (!left->available || !right->available)
        return (0);

    if (!cooldown_ok(left, data))
        return (0);

    if (!cooldown_ok(right, data))
        return (0);

    return (1);
}

int can_take(t_coder *coder, int coder_id)
{
    t_request *top_1;
    t_request *top_2;


    if (coder->left->queue.size == 0 || coder->right->queue.size == 0)
        return (0);
    top_1 = pick_next(coder->left);
    top_2 = pick_next(coder->right);
	if (top_1->coder_id != coder_id || top_2->coder_id != coder_id)
		return (0);
	if (!coder->left->available || !coder->right->available)
		return (0);
	return (1);
}

void	add_request(t_coder	*coder, t_data	*data, t_request	req)
{
	t_dongle	*first;
	t_dongle	*second;
	
	
	get_dongle_order(coder, &first, &second);
	
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	
	heap_push(&first->queue, req, data->scheduler);
	heap_push(&second->queue, req, data->scheduler);
	
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
}

void	remove_request(t_coder	*coder, t_data	*data)
{
	t_dongle	*first;
	t_dongle	*second;


	get_dongle_order(coder, &first, &second);

	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);

	heap_pop(&first->queue, data->scheduler);
	heap_pop(&second->queue, data->scheduler);

	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
}

void	acquire_dongle(t_coder *coder, t_data *data)
{
	t_request	req;


	req.coder_id = coder->id;
	req.left_dongle = coder->left->id;
	req.right_dongle = coder->right->id;
	req.deadline = coder->last_compile + data->time_to_burnout;
	pthread_mutex_lock(&data->counter_mutex);
	req.arrival_order = data->request_counter++;
	pthread_mutex_unlock(&data->counter_mutex);

	add_request(coder, data, req);

	while (is_running(data)
		&& (!can_take(coder, coder->id) || !request_can_run(&req, data)))
	{
		if (can_take(coder, coder->id))
			wait_cooldown(coder->left, data);
		else
			pthread_cond_wait(&coder->left->cond, &coder->left->mutex);
	}
	if (is_running(data))
	{
		remove_request(coder, data);
		log_action(data, coder->id, "has taken a dongle");
		log_action(data, coder->id, "has taken a dongle");

		pthread_mutex_lock(&coder->left->mutex);
		coder->left->available = 0;
		pthread_mutex_unlock(&coder->left->mutex);

		pthread_mutex_lock(&coder->right->mutex);
		coder->right->available = 0;
		pthread_mutex_unlock(&coder->right->mutex);
	}
}