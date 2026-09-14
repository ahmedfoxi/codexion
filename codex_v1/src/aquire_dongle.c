/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aquire_dongle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:02:41 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/14 19:27:31 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


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

static long	get_cooldown_wait(t_coder *coder, t_data *data)
{
	long	left;
	long	right;


	left = cooldown_remaining(coder->left, data);
	right = cooldown_remaining(coder->right, data);
	if (left > right)
		return (left);
	return (right);
}

static void	wait_state_timeout(t_data *data, long wait_ms)
{
	pthread_mutex_unlock(&data->state_mutex);

	usleep(wait_ms * 1000);

	pthread_mutex_lock(&data->state_mutex);
}

// static void	wait_state_timeout(t_data *data, long wait_ms)
// {
// 	struct timespec	ts;


// 	clock_gettime(CLOCK_REALTIME, &ts);

// 	ts.tv_sec += wait_ms / 1000;
// 	ts.tv_nsec += (wait_ms % 1000) * 1000000;

// 	if (ts.tv_nsec >= 1000000000)
// 	{
// 		ts.tv_sec++;
// 		ts.tv_nsec -= 1000000000;
// 	}

// 	pthread_cond_timedwait(
// 		&data->state_cond,
// 		&data->state_mutex,
// 		&ts
// 	);
// 	// ft_usleep(wait_ms, data);
// }

int	cooldown_ok(t_dongle *dongle, t_data *data)
{
	if (dongle->last_release == 0)
		return (1);
	if (get_time_ms() - dongle->last_release >= data->dongle_cooldown)
		return (1);
	return (0);
}

static int can_take_two(t_coder *coder, t_data *data)
{
	t_request *left_top;
	t_request *right_top;

	
	if (coder->left->queue.size == 0 || coder->right->queue.size == 0)
		return (0);
	
	left_top = pick_next(coder->left);
	right_top = pick_next(coder->right);
	
	// printf(
	// 	"coder %d | left_top=%d right_top=%d | left_av=%d right_av=%d\n",
	// 	coder->id,
	// 	left_top->coder_id,
	// 	right_top->coder_id,
	// 	coder->left->available,
	// 	coder->right->available
	// );
	
	if (left_top->coder_id != coder->id ||right_top->coder_id != coder->id)
		return (0);

	if (!coder->left->available || !coder->right->available)
		return (0);

	if (!cooldown_ok(coder->left, data) || !cooldown_ok(coder->right, data))
		return (0);

	return (1);
}

static void	acquire_dongle(t_coder *coder, t_data *data)
{
	t_request	req;
	long	wait_ms;


	req.coder_id = coder->id;
	req.deadline = coder->last_compile + data->time_to_burnout;

	pthread_mutex_lock(&data->counter_mutex);
	req.arrival_order = data->request_counter++;
	pthread_mutex_unlock(&data->counter_mutex);

	pthread_mutex_lock(&data->state_mutex);

	heap_push(&coder->left->queue, req, data->scheduler);
	heap_push(&coder->right->queue, req, data->scheduler);

	while (is_running(data)
		&& !can_take_two(coder, data))
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
