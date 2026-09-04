/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 12:55:03 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/04 20:11:44 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codex.h"


void    start_simulation()
{
    
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
    
    data->start_time = get_time_ms();


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

// void *coder_routing(void *arg)
// {
//     t_coder *coder;

//     coder = (t_data *)arg;
//     printf("%d %d has taken a dongle", coder->compile_count, coder->id);

//     return NULL;
// }

// void create_threads(t_data *data)
// {
//     int i;


//     i = 0;
//     while (i < data->number_of_coders)
//     {
//         pthread_create(
//             &data->coders[i].thread,
//             NULL,
//             coder_routing,
//             &data->coders[i]
//         );
//         i++;
//     }
//     i = 0;
//     while (i < data->number_of_coders)
//         pthread_join(data->coders[i++].thread, NULL);
// }
