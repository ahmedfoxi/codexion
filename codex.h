/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:55:07 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/11 08:57:48 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CODEX_H
#define CODEX_H


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


// typedef struct s_args
// {
//     int     number_of_coders;
//     int     time_to_burnout;
//     int     time_to_compile;
//     int     time_to_debug;
//     int     time_to_refactor;
//     int     number_of_compiles_required;
//     int     dongle_cooldown;
//     char      *scheduler;
// }   t_args;


typedef struct s_request
{
	int		coder_id;
	long	arrival_time;
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
}	t_dongle;


typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	long			last_compile;
	int				compile_count;
	int				running;
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

	t_coder			*coders;
	t_dongle		*dongles;
	t_heap			queue;

	pthread_t		monitor;

	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;

	int				stop;
}	t_data;

long ft_atoi(char *str);
int is_digits(char *str);
t_data    *ft_parse(char **args);
t_data  *get_args(char **args);

#endif
