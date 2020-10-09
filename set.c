/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:27:26 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/09 16:50:08 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int		set_texture(t_mlx *mlx, t_img *texture, char *path)
{
	int fd;

	errno = 0;
	fd = open(path, O_RDONLY);
	if (fd >= 0)
	{
		close(fd);
		texture->img_ptr = mlx_xpm_file_to_image(mlx->mlx_ptr, path,
										&texture->width, &texture->height);
		if (!texture->img_ptr)
			return (FORMAT_ERROR);
		texture->data =
		(int *)mlx_get_data_addr(texture->img_ptr, &texture->bpp,
								&texture->size_l, &texture->endian);
	}
	if (errno)
	{
		perror("Error\nOpen texture");
		return (STANDART_ERROR);
	}
	return (0);
}

int		set_textures(t_settings *settings, t_mlx *mlx)
{
	int code;

	code = 0;
	if ((code = set_texture(mlx, mlx->tex_so, settings->south_tex_path)) < 0)
		return (code);
	if ((code = set_texture(mlx, mlx->tex_we, settings->west_tex_path)) < 0)
		return (code);
	if ((code = set_texture(mlx, mlx->tex_no, settings->north_tex_path)) < 0)
		return (code);
	if ((code = set_texture(mlx, mlx->tex_ea, settings->east_tex_path)) < 0)
		return (code);
	if ((code = set_texture(mlx, mlx->tex_s, settings->sprite_path)) < 0)
		return (code);
	if (settings->floor_tex_path)
	{
		if ((code = set_texture(mlx, mlx->tex_f, settings->floor_tex_path)) < 0)
		{
			return (code);
		}
	}
	return (0);
}

void	get_plane(char c, t_fpos *plane, double pov)
{
	if (c == 'N')
	{
		plane->x = 0.0;
		plane->y = pov;
	}
	else if (c == 'S')
	{
		plane->x = 0.0;
		plane->y = -pov;
	}
	else if (c == 'W')
	{
		plane->x = -pov;
		plane->y = 0.0;
	}
	else if (c == 'E')
	{
		plane->x = pov;
		plane->y = 0.0;
	}
}

void	get_direction(char c, t_fpos *dir)
{
	if (c == 'N')
	{
		dir->x = -1.0;
		dir->y = 0.0;
	}
	else if (c == 'S')
	{
		dir->x = 1.0;
		dir->y = 0.0;
	}
	else if (c == 'W')
	{
		dir->x = 0.0;
		dir->y = -1.0;
	}
	else if (c == 'E')
	{
		dir->x = 0.0;
		dir->y = 1.0;
	}
}

int		set_settings(t_settings *settings, t_mlx *mlx)
{
	int code;

	code = 0;
	mlx->map = &settings->map;
	if (mlx_alloc(mlx, settings) < 0)
		return (MALLOC_ERROR);
	if ((code = set_textures(settings, mlx)) < 0)
		return (code);
	mlx->player.pos.x = (double)settings->player.pos.x;
	mlx->player.pos.y = (double)settings->player.pos.y;
	get_direction(settings->player.dir, &mlx->player.dir);
	get_plane(settings->player.dir, &mlx->player.plane, 0.66);
	if (settings->resolution_x > MAX_WIDTH)
		mlx->img.width = MAX_WIDTH;
	else
		mlx->img.width = settings->resolution_x;
	if (settings->resolution_y > MAX_HEIGHT)
		mlx->img.height = MAX_HEIGHT;
	else
		mlx->img.height = settings->resolution_y;
	mlx->ceil = &settings->color_ceil;
	mlx->floor = &settings->color_floor;
	return (0);
}
