/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:54:27 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/09 15:58:16 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	rotate(t_mlx *mlx, int positive)
{
	double old_dir_x;
	double old_plane_x;
	double rotate;

	if (positive)
		rotate = ROTATE;
	else
		rotate = -ROTATE;
	old_dir_x = mlx->player.dir.x;
	mlx->player.dir.x = mlx->player.dir.x * cos(rotate) -
	mlx->player.dir.y * sin(rotate);
	mlx->player.dir.y = old_dir_x * sin(rotate) +
	mlx->player.dir.y * cos(rotate);
	old_plane_x = mlx->player.plane.x;
	mlx->player.plane.x = mlx->player.plane.x * cos(rotate) -
	mlx->player.plane.y * sin(rotate);
	mlx->player.plane.y = old_plane_x * sin(rotate) +
	mlx->player.plane.y * cos(rotate);
}

void	move(t_mlx *mlx, int sign)
{
	if (mlx->map->map_ptr[(int)(mlx->player.pos.x + sign * mlx->player.dir.x *
		VELOCITY)][(int)mlx->player.pos.y] != '1')
		mlx->player.pos.x += sign * mlx->player.dir.x * VELOCITY;
	if (mlx->map->map_ptr[(int)mlx->player.pos.x][(int)(mlx->player.pos.y +
		sign * mlx->player.dir.y * VELOCITY)] != '1')
		mlx->player.pos.y += sign * mlx->player.dir.y * VELOCITY;
}

int		key(int keycode, t_mlx *mlx)
{
	int code;

	if (keycode == 53)
	{
		win_close(mlx);
	}
	else if (keycode == 13)
		move(mlx, 1);
	else if (keycode == 0)
		rotate(mlx, 1);
	else if (keycode == 1)
		move(mlx, -1);
	else if (keycode == 2)
		rotate(mlx, 0);
	reset_image(mlx);
	code = draw(mlx);
	if (code < 0)
	{
		print_error(code);
		win_close(mlx);
	}
	return (0);
}

void	dda_alg(t_mlx *mlx, t_ray *ray)
{
	int hit;

	hit = 0;
	while (!hit)
	{
		if (ray->side.x < ray->side.y)
		{
			ray->side.x += ray->delta.x;
			ray->map.x += ray->step.x;
			ray->world_side = (ray->dir.x > 0) ? 'S' : 'N';
		}
		else
		{
			ray->side.y += ray->delta.y;
			ray->map.y += ray->step.y;
			ray->world_side = (ray->dir.y > 0) ? 'E' : 'W';
		}
		if (mlx->map->map_ptr[ray->map.x][ray->map.y] == '1')
			hit = 1;
	}
}

void	start_game(t_mlx *mlx, int save)
{
	int code;

	code = draw(mlx);
	if (code < 0)
	{
		print_error(code);
		win_close(mlx);
	}
	if (save == 1)
	{
		bitmap(mlx);
		exit(0);
	}
	mlx_hook(mlx->win, 2, 0, key, mlx);
	mlx_hook(mlx->win, 17, 1LU << 17u, win_close, mlx);
	mlx_do_sync(mlx->mlx_ptr);
	mlx_loop(mlx->mlx_ptr);
}
