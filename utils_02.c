/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_02.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:19:54 by ctobias           #+#    #+#             */
/*   Updated: 2020/11/18 14:07:41 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int		mlx_alloc(t_mlx *mlx, t_settings *settings)
{
	if (settings->floor_tex_path)
	{
		if (!(mlx->tex_f = malloc(sizeof(t_img))))
			return (MALLOC_ERROR);
	}
	if (!(mlx->tex_no = malloc(sizeof(t_img))))
		return (MALLOC_ERROR);
	if (!(mlx->tex_so = malloc(sizeof(t_img))))
		return (MALLOC_ERROR);
	if (!(mlx->tex_we = malloc(sizeof(t_img))))
		return (MALLOC_ERROR);
	if (!(mlx->tex_ea = malloc(sizeof(t_img))))
		return (MALLOC_ERROR);
	if (!(mlx->tex_s = malloc(sizeof(t_img))))
		return (MALLOC_ERROR);
	mlx->num_sprites = count_sprites(mlx);
	if (!(mlx->sprites = malloc(sizeof(t_sprite) * mlx->num_sprites)))
		return (MALLOC_ERROR);
	return (0);
}

int		rgb_to_int(t_color_rgb *color_rgb)
{
	int color;

	color = 0;
	color = color_rgb->r;
	color = color << 8;
	color = color | color_rgb->g;
	color = color << 8;
	color = color | color_rgb->b;
	return (color);
}

void	mlx_free(t_mlx *mlx)
{
	ft_free(mlx->tex_so);
	ft_free(mlx->tex_no);
	ft_free(mlx->tex_we);
	ft_free(mlx->tex_ea);
	ft_free(mlx->tex_s);
	ft_free(mlx->tex_f);
	ft_free(mlx->sprites);
	array2d_clear(mlx->map->map_ptr, mlx->map->height);
	ft_free(mlx->mlx_ptr);
}

void	mlx_struct_init(t_mlx *mlx)
{
	mlx->ceil = NULL;
	mlx->tex_f = NULL;
	mlx->floor = NULL;
	mlx->mlx_ptr = NULL;
	mlx->tex_ea = NULL;
	mlx->tex_no = NULL;
	mlx->tex_so = NULL;
	mlx->tex_s = NULL;
	mlx->tex_we = NULL;
	mlx->map = NULL;
	mlx->perp_buff = NULL;
	mlx->sprites = NULL;
	mlx->num_sprites = 0;
}

int		count_sprites(t_mlx *mlx)
{
	int i;
	int j;
	int count;

	i = 0;
	count = 0;
	while (i < mlx->map->height)
	{
		j = 0;
		while (j < mlx->map->width)
		{
			if (mlx->map->map_ptr[i][j] == '2')
				++count;
			++j;
		}
		++i;
	}
	return (count);
}
