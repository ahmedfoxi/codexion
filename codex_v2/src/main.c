/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:55:03 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/17 23:54:02 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"


int main(int ac, char **av)
{
    t_data  *data;


    if (ac != 9)
    {
        write(2, "ERROR", 5);
        return 0;
    }
    data = ft_parse(av);
    ft_coders_init(data);
    ft_dongles_init(data);

    return (0);
}

// void *monitor_routing(void *arg)
// {
//     t_data *data;


//     data = (t_data *)arg;
//     while (!data->stop)
//     {
//         //check last_compile and count_compile
//     }

//     return NULL;
// }

// void merge(t_data *data)
// {
//     // initialization
//     coders_init(data);
//     dongles_init(data);
//     create_threads(data);

//     // create monitor thread
//     pthread_create(&data->monitor, NULL, monitor_routing, data);
//     pthread_join(data->monitor, NULL);

// }
