/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:55:07 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/08 15:46:15 by ahbarbou         ###   ########.fr       */
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
	int		coder_id;
	long	arrival_order;
	long	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	*requests;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;
	long			available_at;
	long			last_release;
	t_heap			queue; // the queue must be pointer
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;

	int 			left_idx;
	int 			right_idx;

	pthread_mutex_t coder_mutex;
	long			last_compile;
	int				compile_count;
	struct s_data	*data;
}	t_coder;

typedef struct s_data
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	int				scheduler;

	long			start_time;

	t_coder			*coders;
	t_dongle		*dongles;

	int				request_counter;

	pthread_t		monitor;

	pthread_mutex_t	log_mutex;
	pthread_mutex_t	simulation_mutex;
	pthread_mutex_t counter_mutex;

	int				runing;
}	t_data;

void affiche(t_data *data); //for test


long	ft_atoi(char *str);
int		is_digits(char *str);
t_data	*ft_parse(char **args);
t_data	*get_args(char **args);
void    start_simulation(t_data *data);


void	coder_init(t_data *data);
void	dongle_init(t_data *data);
void	link_coder_dongle(t_data *data);


void	*coder_routing(void *arg);

// time calcule

long	get_time_ms(void);
void	ft_usleep(long ms, t_data *data);
long    elapsed_ms(t_data   *data);

// heap queue functions

int     compare_requests(t_request a, t_request b, int scheduler);
void    heap_swap(t_request *a, t_request *b);
void    heap_init(t_heap *heap, int capacity);
void    heap_push(t_heap *heap, t_request req, int scheduler);
t_request	heap_pop(t_heap *heap, int scheduler);
// t_request *pick_next(t_data *data);
t_request *pick_next(t_dongle *d);

void    clean_up(t_data *data);

// aquire dongle

int	cooldown_ok(t_dongle *dongle, t_data *data);
int	can_take(t_dongle *dongle, int coder_id);
void	wait_cooldown(t_dongle *dongle, t_data *data);
void	acquire_dongle(t_coder *coder, t_data *data, t_dongle *dongle);
void	take_dongles(t_coder *coder, t_data *data);

// dongle utils

void	release_dongle(t_dongle *dongle);
void	release_dongles(t_coder *coder);
void	get_dongle_order(t_coder *coder, t_dongle **first, t_dongle **second);

// routing

int	compile_cycle(t_coder *coder, t_data *data);
void	*coder_routine(void *arg);

// monitor

void	*monitor_routine(void *arg);
void    check_burnout(t_data *data);
void	check_all_done(t_data *data);
void    handle_bournout(t_data   *data, int  idx, long   now);

// log action
void    log_action(t_data *data, int id, char   *msg);
int is_running(t_data *data);


//
void	wake_all(t_data *data);




#endif
