/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:07:33 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/09 15:53:15 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->last_release = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder->left);
	release_dongle(coder->right);
}

void	get_dongle_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_idx <= coder->right_idx)
	{
		// printf("%d <= %d", coder->left_idx, coder->right_idx);
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		// printf("%d > %d", coder->left_idx, coder->right_idx);
		*first = coder->right;
		*second = coder->left;
	}
}
