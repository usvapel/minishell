/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_arena.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:04 by erantala          #+#    #+#             */
/*   Updated: 2025/07/12 16:30:19 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Memory arena to store all allocated memory in one location.
void	*arena_malloc(size_t n)
{
	t_arena	*arena;
	void	*ret;
	size_t	alg_i;

	arena = find_arena(n);
	alg_i = (arena->index + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
	while (arena->max - alg_i < n)
		arena = find_arena(n + (alg_i - arena->index));
	ret = &arena->data[alg_i];
	arena->index = alg_i + n;
	return (ret);
}

// gets the next available arena and returns a pointer to the free memory
t_arena	*init_arena(size_t size)
{
	t_arena	*arena;

	arena = ft_calloc(1, size + (sizeof(t_arena)));
	if (!arena)
		ft_exit("MALLOC FAILURE", 1);
	arena->max = size;
	arena->index = 0;
	return (arena);
}

// Returns a free point in the arena, if arenas are full allocates more.
t_arena	*find_arena(size_t n)
{
	static int	arena_count = 2;
	int			i;
	t_arena		**arenas;

	i = 0;
	arenas = get_arenas(NULL);
	while (i < arena_count)
	{
		if (arenas[i] && arenas[i]->max - arenas[i]->index >= n)
			return (arenas[i]);
		i++;
	}
	if (i == arena_count)
	{
		arenas = new_arena(arenas, arena_count, n);
		arena_count++;
	}
	return (arenas[i]);
}

// Creates a new arena when there isn't enough memory in current arenas.
// Free the old arena pointers, only the data matters.
t_arena	**new_arena(t_arena **curr, int count, size_t n)
{
	t_arena	**arenas;
	int		i;

	i = 0;
	arenas = ft_calloc(sizeof(t_arena *), count + 2);
	if (!arenas)
		ft_exit("MALLOC FAILURE", 1);
	while (i < count)
	{
		arenas[i] = curr[i];
		i++;
	}
	if (n > ARENA_SIZE)
		arenas[i] = init_arena(n);
	else
		arenas[i] = init_arena(ARENA_SIZE);
	arenas[i + 1] = NULL;
	get_arenas(arenas);
	free(curr);
	return (arenas);
}

t_arena	**get_arenas(t_arena **new)
{
	static t_arena	**arenas = NULL;

	if (!arenas)
	{
		arenas = ft_calloc(sizeof(t_arena *), 2);
		if (!arenas)
			ft_exit("MALLOC FAILURE", 1);
		arenas = new_arena(arenas, 0, ARENA_SIZE);
		arenas = new_arena(arenas, 1, ARENA_SIZE);
	}
	if (new)
		arenas = new;
	return (arenas);
}
