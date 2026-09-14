/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_calule.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 14:45:00 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/14 18:13:08 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


long long    get_time_ms(void)
{
    struct timeval  tv;


    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	ft_usleep(long long ms, t_data *data)
{
	long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
	{
		if (!is_running(data))
			break ;
		usleep(1000);
	}
}

long long    elapsed_ms(t_data   *data)
{
    return (get_time_ms() - data->start_time);
}
