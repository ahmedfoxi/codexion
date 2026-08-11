/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 12:15:10 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/08 12:48:54 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"


pthread_t     *thread_engine(int number_of_coders)
{
    pthread_t   *threads;
    int     i;
    
    
    i = 0;
    threads = malloc(sizeof(pthread_t) * number_of_coders);
    if (!threads)
        return (NULL);

    while (i < number_of_coders)
    {
        pthread_create(&threads[i], NULL, NULL, NULL);
        i++;
    }
    return (threads);    
}

