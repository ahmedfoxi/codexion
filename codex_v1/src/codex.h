/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:55:07 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/16 00:33:14 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEX_H
# define CODEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_request
{
	int			coder_id;
	long long	arrival_order;
	long long	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	*requests;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int			id;
	int			available;
	long long	last_release;
	t_heap		queue;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	// int				left_idx;
	// int				right_idx;
	pthread_mutex_t	coder_mutex;
	long long		last_compile;
	int				compile_count;
	struct s_data	*data;
}	t_coder;

typedef struct s_data
{
	int				number_of_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				number_of_compiles_required;
	long long		dongle_cooldown;
	int				scheduler;
	long long		start_time;
	t_coder			*coders;
	t_dongle		*dongles;
	int				request_counter;
	pthread_t		monitor;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	simulation_mutex;
	pthread_mutex_t	counter_mutex;
	pthread_mutex_t	state_mutex;
	pthread_cond_t	state_cond;
	int				runing;
}	t_data;

void		ft_parse(char **args, t_data *data);

int			init_data(t_data *data);
t_request	create_request(t_coder	*coder, t_data	*data);

long long	get_time_ms(void);
void		ft_usleep(long long ms, t_data *data);
long long	elapsed_ms(t_data *data);

int			heap_init(t_heap *heap, int capacity);
void		heap_push(t_heap *heap, t_request req, int scheduler);
t_request	heap_pop(t_heap *heap, int scheduler);
t_request	*pick_next(t_dongle *d);

void		clean_up(t_data *data);
long		get_cooldown_wait(t_coder *coder, t_data *data);
void		take_dongles(t_coder *coder, t_data *data);
void		release_dongles(t_coder *coder, t_data *data);

void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);
void		wake_all(t_data *data);
void	check_all_done(t_data *data);

void		log_action(t_data *data, int id, char *msg);
int			is_running(t_data *data);

#endif
