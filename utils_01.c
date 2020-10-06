/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:21:18 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/06 17:23:06 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void		mem_set_char(char **ptr, char c, int size)
{
	int i;

	i = 0;
	if (ptr && *ptr)
	{
		while (i < size)
		{
			(*ptr)[i] = c;
			++i;
		}
	}
}

int			is_valid_map_symbol(char c)
{
	char *objects;
	char *player;

	objects = "012 ";
	player = "NWSE";
	if (ft_strchr(objects, c))
		return (1);
	else if (ft_strchr(player, c))
		return (2);
	return (0);
}

void		array2d_clear(char **array, int width, int height)
{
	int i;

	i = 0;
	while (i < height)
	{
		ft_free(array[i]);
		++i;
	}
	ft_free(array);
}

int			check_settings(t_settings *settings)
{
	if ((settings->color_ceil.r >= 0 && settings->color_ceil.g >= 0 &&
		settings->color_ceil.b >= 0) &&
		((settings->color_floor.r >= 0 && settings->color_floor.g >= 0 &&
		settings->color_floor.b >= 0) || settings->floor_tex_path) &&
		settings->east_tex_path && settings->north_tex_path &&
		settings->south_tex_path && settings->west_tex_path &&
		settings->sprite_path && settings->resolution_x > 0 &&
		settings->resolution_y > 0)
		return (1);
	return (0);
}

int			check_format(char *file, char *format)
{
	char *p;

	p = ft_strrchr(file, '.');
	if (p && ft_strncmp(p, format,
		ft_max(ft_strlen(format), ft_strlen(p))) == 0)
		return (1);
	return (0);
}
