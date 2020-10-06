/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:29:27 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/06 17:31:45 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int			win_close(t_mlx *mlx)
{
	mlx_destroy_window(mlx->mlx_ptr, mlx->win);
	mlx_free(mlx);
	exit(0);
	return (0);
}

int			clear_screen(t_mlx *mlx)
{
	int		x;
	int		y;
	int		color_floor;
	int		color_ceil;

	color_floor = 0;
	color_ceil = 0;
	if (mlx->ceil)
		color_ceil = rgb_to_int(mlx->ceil);
	if (mlx->floor)
		color_floor = rgb_to_int(mlx->floor);
	y = 0;
	while (y < mlx->img.height)
	{
		x = 0;
		while (x < mlx->img.width)
		{
			if (y < mlx->img.height / 2)
				mlx->img.data[y * mlx->img.width + x] = color_ceil;
			else
				mlx->img.data[y * mlx->img.width + x] = color_floor;
			++x;
		}
		++y;
	}
	return (0);
}

int			canvas_init(t_mlx *mlx, t_settings *settings)
{
	mlx->mlx_ptr = mlx_init();
	if (set_settings(settings, mlx) < 0)
	{
		mlx->mlx_ptr = NULL;
		return (-1);
	}
	mlx->win = mlx_new_window(mlx->mlx_ptr, mlx->img.width, mlx->img.height, "Example1");
	if (!mlx->win)
		return (MALLOC_ERROR);
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, mlx->img.width, mlx->img.height);
	if (!mlx->img.img_ptr)
		return (MALLOC_ERROR);
	mlx->img.data = (int *)mlx_get_data_addr(mlx->img.img_ptr, &mlx->img.bpp, &mlx->img.size_l,
										&mlx->img.endian);
	if (!mlx->img.data)
		return (MALLOC_ERROR);
	if (NULL == mlx->img.data)
		return (MALLOC_ERROR);
	return(0);
}

void		reset_image(t_mlx *mlx)
{
	mlx_destroy_image(mlx->mlx_ptr, mlx->img.img_ptr);
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, mlx->img.width, mlx->img.height);
}

int			draw(t_mlx *mlx)
{
	t_ipos range;


	range.x = mlx->img.height / 2;
	range.y = mlx->img.height;
	if (!(mlx->perp_buff = malloc(sizeof(double) * mlx->img.width)))
		return (MALLOC_ERROR);
	clear_screen(mlx);
	if (mlx->tex_f != NULL)
		draw_floor(mlx, &range);
	draw_walls(mlx);
	if (draw_sprites(mlx, mlx->sprites, mlx->num_sprites) < 0)
		return (MALLOC_ERROR);
	
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win, mlx->img.img_ptr, 0, 0);
	free(mlx->perp_buff);
	return (0);
}