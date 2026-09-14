/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:07:33 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/14 18:08:31 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


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
