/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbarbou <ahbarbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 12:15:10 by ahbarbou          #+#    #+#             */
/*   Updated: 2026/09/15 14:31:38 by ahbarbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

int	compare_requests(t_request a, t_request b, int scheduler)
{
	if (scheduler == 0)
	{
		if (a.arrival_order != b.arrival_order)
			return (a.arrival_order > b.arrival_order);
		return (a.coder_id < b.coder_id);
	}
	if (a.deadline != b.deadline)
		return (a.deadline < b.deadline);
	if (a.arrival_order != b.arrival_order)
		return (a.arrival_order < b.arrival_order);
	return (a.coder_id < b.coder_id);
}

void	heap_swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_push(t_heap *heap, t_request req, int scheduler)
{
	int	i;
	int	parent;

	if (heap->size >= heap->capacity)
		return ;
	i = heap->size++;
	heap->requests[i] = req;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (compare_requests(heap->requests[i],
				heap->requests[parent], scheduler))
		{
			heap_swap(&heap->requests[i], &heap->requests[parent]);
			i = parent;
		}
		else
			break ;
	}
}

static void	heapify_down(t_heap *heap, int i, int scheduler)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < heap->size
			&& compare_requests(heap->requests[left],
				heap->requests[smallest], scheduler))
			smallest = left;
		if (right < heap->size
			&& compare_requests(heap->requests[right],
				heap->requests[smallest], scheduler))
			smallest = right;
		if (smallest == i)
			break ;
		heap_swap(&heap->requests[i], &heap->requests[smallest]);
		i = smallest;
	}
}

t_request	heap_pop(t_heap *heap, int scheduler)
{
	t_request	min_req;

	min_req = heap->requests[0];
	heap->requests[0] = heap->requests[--heap->size];
	heapify_down(heap, 0, scheduler);
	return (min_req);
}
