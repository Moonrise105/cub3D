#include "./mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libft/libft.h"
#include "./cub.h"

#define VELOCITY 0.51
#define ROTATE 0.2

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

int clear_screen(t_mlx *mlx)
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
			if (ray->dir.x > 0)
				ray->world_side = 'S';
			else
				ray->world_side = 'N';
		}
		else
		{
			ray->side.y += ray->delta.y;
			ray->map.y += ray->step.y;
			if (ray->dir.y > 0)
				ray->world_side = 'E';
			else
				ray->world_side = 'W';
		}
		if (mlx->map->map_ptr[ray->map.x][ray->map.y] == '1')
			hit = 1;
	}	
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




int		canvas_init(t_mlx *mlx, t_settings *settings)
{
	mlx->mlx_ptr = mlx_init();
	if (set_settings(settings, mlx) < 0)
	{
		mlx->mlx_ptr = NULL;
		return (-1);
	}
	mlx->win = mlx_new_window(mlx->mlx_ptr, mlx->img.width, mlx->img.height, "Example1");
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, mlx->img.width, mlx->img.height);
	mlx->img.data = (int *)mlx_get_data_addr(mlx->img.img_ptr, &mlx->img.bpp, &mlx->img.size_l,
										&mlx->img.endian);
	return(0);
}

void reset_image(t_mlx *mlx)
{
	mlx_destroy_image(mlx->mlx_ptr, mlx->img.img_ptr);
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, mlx->img.width, mlx->img.height);
}

int		draw(t_mlx *mlx)
{
	t_ipos range;


	range.x = mlx->img.height / 2;
	range.y = mlx->img.height;
	mlx->perp_buff = malloc(sizeof(double) * mlx->img.width);

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
	mlx->player.dir.x = mlx->player.dir.x * cos(rotate) - mlx->player.dir.y * sin(rotate);
	mlx->player.dir.y = old_dir_x * sin(rotate) + mlx->player.dir.y * cos(rotate);
	old_plane_x = mlx->player.plane.x;
	mlx->player.plane.x = mlx->player.plane.x * cos(rotate) - mlx->player.plane.y * sin(rotate);
	mlx->player.plane.y = old_plane_x * sin(rotate) + mlx->player.plane.y * cos(rotate);

}

int key(int keycode, t_mlx *mlx)
{
	int code;

	if (keycode == 53)
	{
		mlx_destroy_window(mlx->mlx_ptr, mlx->win);
		exit(1);
	}

	else if (keycode == 13)
	{
		if (mlx->map->map_ptr[(int)(mlx->player.pos.x + mlx->player.dir.x * VELOCITY)][(int)mlx->player.pos.y] != '1')
			mlx->player.pos.x += mlx->player.dir.x * VELOCITY;
		if (mlx->map->map_ptr[(int)mlx->player.pos.x][(int)(mlx->player.pos.y + mlx->player.dir.y * VELOCITY)] != '1')
			mlx->player.pos.y += mlx->player.dir.y * VELOCITY;
	}
	else if (keycode == 0)
		rotate(mlx, 1);
	else if (keycode == 1)
	{
		if (mlx->map->map_ptr[(int)(mlx->player.pos.x - mlx->player.dir.x * VELOCITY)][(int)mlx->player.pos.y] != '1')
			mlx->player.pos.x -= mlx->player.dir.x * VELOCITY;
		if (mlx->map->map_ptr[(int)(mlx->player.pos.x)][(int)(mlx->player.pos.y - mlx->player.dir.y * VELOCITY)] != '1')
			mlx->player.pos.y -= mlx->player.dir.y * VELOCITY;
	}
	else if (keycode == 2)
		rotate(mlx, 0);
	reset_image(mlx);
	code = draw(mlx);
	if (code < 0)
	{
		print_error(code);
		mlx_destroy_window(mlx->mlx_ptr, mlx->win);
		exit(-1);
	}
	return (0);
}






void	start_game(t_mlx *mlx, int save)
{
	draw(mlx);
	if (save == 1)
	{
		bitmap(mlx);
		exit(0);
	}
	mlx_hook(mlx->win, 2, 0, key, mlx);
	mlx_do_sync(mlx->mlx_ptr);
	mlx_loop(mlx->mlx_ptr);
}

void	mlx_free(t_mlx *mlx)
{
	ft_free(&mlx->tex_SO);
	ft_free(&mlx->tex_NO);
	ft_free(&mlx->tex_WE);
	ft_free(&mlx->tex_EA);
	ft_free(&mlx->tex_s);
	ft_free(&mlx->tex_f);
	ft_free(mlx->sprites);
	ft_free(mlx->perp_buff);
}

int		cube(t_mlx *mlx, t_settings *settings)
{
	
	if (parser(settings) < 0)
	{
		parser_free(settings);
		return (-1);
	}
	if (canvas_init(mlx, settings) < 0)
		return (-1);
	
	start_game(mlx, settings->save);
	parser_free(settings);
	mlx_free(mlx);
	return (0);
}


void	mlx_struct_init(t_mlx *mlx)
{
	mlx->ceil = NULL;
	mlx->tex_f = NULL;
	mlx->floor = NULL;
	mlx->mlx_ptr = NULL;
	mlx->tex_EA = NULL;
	mlx->tex_NO = NULL;
	mlx->tex_SO = NULL;
	mlx->tex_s = NULL;
	mlx->tex_WE = NULL;
	mlx->map = NULL;
	mlx->perp_buff = NULL;
	mlx->sprites = NULL;
	mlx->num_sprites = 0;
}


int		cube_init(char *file, int save)
{
	t_mlx mlx;
	t_settings *settings;

	settings = (t_settings *)malloc(sizeof(t_settings));
	mlx_struct_init(&mlx);
	init_settings(settings);
	settings->file = file;
	settings->save = save;
	//mlx = malloc(sizeof(t_mlx));
	cube(&mlx, settings);
	
	
	return (0);
}

int main(int ac, char **av)
{
	int save;

	save = 0;
	if (ac > 1)
	{
		if (!check_format(av[1], ".cub"))
		{
			print_error(FORMAT_ERROR);
			return(FORMAT_ERROR);
		}
		if (ac > 2 && ft_strncmp(av[2], "--save", 6) == 0)
			save = 1;
		cube_init(av[1], save);
	}
	return (0);
}