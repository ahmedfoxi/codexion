/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:55:03 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/07 23:42:01 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void	wake_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

void    start_simulation(t_data *data)
{
    int i;


    data->start_time = get_time_ms();
    i = 0;
    while (i < data->number_of_coders)
        data->coders[i++].last_compile = data->start_time;

    data->runing = 1;


    pthread_create(&data->monitor, NULL, monitor_routine, data);
    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]);
        i++;
    }

    pthread_join(data->monitor, NULL);
    wake_all(data);
    i = 0;
    while (i < data->number_of_coders)
        pthread_join(data->coders[i++].thread, NULL);

}

int main(int ac, char **av)
{
    t_data  *data;


    if (ac != 9)
    {
        write(2, "ERROR", 5);
        return 0;
    }
    data = ft_parse(av);
    dongle_init(data);
    coder_init(data);
    link_coder_dongle(data);
    affiche(data);
    printf("start simulation");
    start_simulation(data);
    clean_up(data);

    return (0);
}
