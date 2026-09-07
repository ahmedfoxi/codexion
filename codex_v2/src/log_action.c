/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_action.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 16:38:44 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/07 22:49:12 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void    log_action(t_data *data, char   *msg)
{
    pthread_mutex_lock(&data->log_mutex);
    printf("%s", msg);
    pthread_mutex_unlock(&data->log_mutex);
}

int is_running(t_data *data)
{
    int run;


    pthread_mutex_lock(&data->simulation_mutex);
    run = data->runing;
    pthread_mutex_unlock(&data->simulation_mutex);
    return (run);
}
