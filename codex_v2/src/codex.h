/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:55:07 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/19 20:01:43 by ahbarbou         ###   ########.fr       */
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
	int 	dongle_left;
	int 	dongle_right;
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
	t_heap			queue; // the queue must be pointer
}	t_dongle;

typedef struct s_coder
{
	int						id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;

	pthread_mutex_t state_mutex;
	long			last_compile;
	int				compile_count;
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

	pthread_t		monitor;

	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;

	int				stop;
}	t_data;

void affiche(t_data *data); //for test

long	ft_atoi(char *str);
int	is_digits(char *str);
t_data	*ft_parse(char **args);
t_data	*get_args(char **args);

void	coder_init(t_data *data);
void	dongle_init(t_data *data);

void	*coder_routing(void *arg);
void	create_threads(t_data *data);

long	get_time_ms(void);
void	precise_sleep(long ms, t_data *data);

// void    print_log(t_coder *coder, const char *action);
// static void	wait_for_dongle(t_dongle *dongle, t_coder *coder);
// void	take_single_dongle(t_dongle *dongle, t_coder *coder);
// void	take_both_dongles(t_coder *coder);

// void	release_single_dongle(t_dongle *dongle, t_data *data);
// void	release_both_dongles(t_coder *coder);

// void	*coder_routine(void *arg);


#endif
