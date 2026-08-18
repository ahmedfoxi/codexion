/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 12:15:10 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/08/18 16:20:34 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"


int     compare_request(t_request a, t_request b, int scheduler)
{
    // fifo
    if (scheduler == 0)
    {
        if (a.arrival_time != b.arrival_time)
            return (a.arrival_time < b.arrival_time);
        return (a.coder_id < b.coder_id);
    }
    // edf
    if (a.deadline != b.deadline)
        return (a.deadline < b.deadline);
    if (a.arrival_time != b.arrival_time)
        return (a.arrival_time < b.arrival_time);
    return (a.coder_id < b.coder_id);
}

void    heap_swap(t_request *a, t_request *b)
{
    t_request   tmp;


    tmp = *a;
    *a = *b;
    *b = tmp;
}

int     heap_init(t_heap *heap, int capacity)
{
    heap->capacity = capacity;
    heap->size = 0;
    heap->requests = malloc(sizeof(t_request) * capacity);
    if (!heap->requests)
        return (0);
    return (1);
}

void    heap_push(t_heap *heap, t_request req, int scheduler)
{
    int     i;
    int     parent;


    if (heap->size >= heap->capacity)
        return ;
    i = heap->size++;
    heap->requests[i] = req;

    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (compare_request(heap->requests[i], heap->requests[parent], scheduler))
        {
            heap_swap(&heap->requests[i], &heap->requests[parent]);
            i = parent;
        }
        else
            break;
    }
}

t_request	heap_pop(t_heap *heap, int scheduler)
{
	t_request	min_req;
	int			i;
	int			left;
	int			right;
	int			smallest;


	min_req = heap->requests[0];
	heap->requests[0] = heap->requests[--heap->size];
	i = 0;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < heap->size && compare_requests(heap->requests[left], heap->requests[smallest], scheduler))
			smallest = left;
		if (right < heap->size && compare_requests(heap->requests[right], heap->requests[smallest], scheduler))
			smallest = right;
		if (smallest != i)
		{
			heap_swap(&heap->requests[i], &heap->requests[smallest]);
			i = smallest;
		}
		else
			break ;
	}
	return (min_req);
}
