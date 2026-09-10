/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 15:41:05 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/10 12:38:07 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


int	compile_cycle(t_coder *coder, t_data *data)
{
	take_dongles(coder, data);
	if (!is_running(data))
		return (0);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile = get_time_ms();
	pthread_mutex_unlock(&coder->coder_mutex);
	log_action(data, coder->id, "is compiling");
	ft_usleep(data->time_to_compile, data);
	release_dongles(coder);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)arg;
	data = coder->data;
	while (is_running(data)
		&& coder->compile_count < data->number_of_compiles_required)
	{
		if (!compile_cycle(coder, data))
			break ;
		if (!is_running(data))
			break ;
		log_action(data, coder->id, "is debugging");
		ft_usleep(data->time_to_debug, data);
		log_action(data, coder->id, "is refactoring");
		ft_usleep(data->time_to_refactor, data);
	}
	return (NULL);
}
