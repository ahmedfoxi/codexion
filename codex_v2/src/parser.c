/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 13:00:19 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/06 16:05:15 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"


int is_digits(char *str)
{
    int i;


    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

long ft_atoi(char *str)
{
    int i;
    long result;


    i = 0;
    result = 0;
    while (str[i])
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    if (result > 2147483647)
    {
        write(2, "ERROR", 5);
        exit(1);
    }

    return (result);
}

t_data    *ft_parse(char **args)
{
    int    i;


    i = 1;
    while (args[i] && i < 8)
    {
        if (!is_digits(args[i]))
        {
            write(2, "ERROR", 5);
            exit(1);
        }
        i++;
    }
    if (strcmp(args[8], "fifo") != 0 && strcmp(args[8], "edf") != 0)
    {
        write(2, "ERROR", 5);
        exit(1);
    }
    return get_args(args);
}

t_data  *get_args(char **args)
{
    t_data *data;
    int     i;
    long     numbers[7];


    data = malloc(sizeof(t_data));
    if (!data)
        return (NULL);

    i = 1;
    while(i < 8)
    {
        numbers[i - 1] = ft_atoi(args[i]);
        i++;
    }
    data->number_of_coders = numbers[0];
    data->time_to_burnout = numbers[1];
    data->time_to_compile = numbers[2];
    data->time_to_debug = numbers[3];
    data->time_to_refactor = numbers[4];
    data->number_of_compiles_required = numbers[5];
    data->dongle_cooldown = numbers[6];
    if (strcmp(args[8], "fifo") == 0)
        data->scheduler = 0;
    if (strcmp(args[8], "edf") == 0)
        data->scheduler = 1;
    data->request_counter = 0;
    data->runing = 1;

    return (data);
}
