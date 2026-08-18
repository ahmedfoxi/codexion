/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 15:19:59 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/17 17:04:13 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void    print_log(t_coder *coder, const char *action)
{
    long    timestamp;


    pthread_mutex_lock(&coder->data->print_mutex);
    pthread_mutex_lock(&coder->data->stop_mutex);
    if (!coder->data->stop)
    {
        timestamp = get_time_ms();
        printf("%ld %d %s\n", timestamp, coder->id, action);
    }
    pthread_mutex_unlock(&coder->data->stop_mutex);
    pthread_mutex_unlock(&coder->data->print_mutex);
}

static void	wait_for_dongle(t_dongle *dongle, t_coder *coder)
{
	long	now;
	long	wait_time;

	while (1)
	{
		pthread_mutex_lock(&coder->data->stop_mutex);
		if (coder->data->stop)
		{
			pthread_mutex_unlock(&coder->data->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->data->stop_mutex);

		now = get_time_ms();
		if (dongle->available && dongle->queue.requests[0].coder_id == coder->id)
		{
			if (now >= dongle->available_at)
				break ;
			/* Handle remaining cooldown time */
			wait_time = dongle->available_at - now;
			pthread_mutex_unlock(&dongle->mutex);
			precise_sleep(wait_time, coder->data);
			pthread_mutex_lock(&dongle->mutex);
			continue ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
}

void	take_single_dongle(t_dongle *dongle, t_coder *coder)
{
	t_request	req;

	req.coder_id = coder->id;
	req.arrival_time = get_time_ms();

	pthread_mutex_lock(&coder->state_mutex);
	req.deadline = coder->last_compile + coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);

	pthread_mutex_lock(&dongle->mutex);
	heap_push(&dongle->queue, req, coder->data->scheduler);

	wait_for_dongle(dongle, coder);

	pthread_mutex_lock(&coder->data->stop_mutex);
	if (!coder->data->stop)
	{
		heap_pop(&dongle->queue, coder->data->scheduler);
		dongle->available = 0;
	}
	pthread_mutex_unlock(&coder->data->stop_mutex);

	pthread_mutex_unlock(&dongle->mutex);
	print_log(coder, "has taken a dongle");
}

void	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	/* Deadlock prevention: Lock dongles in ascending ID order */
	if (coder->left->id < coder->right->id)
	{
		first = coder->left;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second = coder->left;
	}
	take_single_dongle(first, coder);
	take_single_dongle(second, coder);
}

void	release_single_dongle(t_dongle *dongle, t_data *data)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->available_at = get_time_ms() + data->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_both_dongles(t_coder *coder)
{
	release_single_dongle(coder->left, coder->data);
	release_single_dongle(coder->right, coder->data);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->data->number_of_coders == 1)
	{
		take_single_dongle(coder->left, coder);
		precise_sleep(coder->data->time_to_burnout, coder->data);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&coder->data->stop_mutex);
		if (coder->data->stop)
		{
			pthread_mutex_unlock(&coder->data->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->data->stop_mutex);

		take_both_dongles(coder);

		/* Start Compiling */
		pthread_mutex_lock(&coder->state_mutex);
		coder->last_compile = get_time_ms();
		coder->compile_count++;
		pthread_mutex_unlock(&coder->state_mutex);

		print_log(coder, "is compiling");
		precise_sleep(coder->data->time_to_compile, coder->data);

		release_both_dongles(coder);

		/* Debugging */
		print_log(coder, "is debugging");
		precise_sleep(coder->data->time_to_debug, coder->data);

		/* Refactoring */
		print_log(coder, "is refactoring");
		precise_sleep(coder->data->time_to_refactor, coder->data);
	}
	return (NULL);
}
