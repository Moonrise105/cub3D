#include "./mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libft/libft.h"
#include "./cub.h"

#define VELOCITY 0.51
#define ROTATE 0.2



void swap(t_sprite *a, t_sprite *b)
{
	t_sprite tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void merge(t_sprite *sprites, int l, int r, int mid)
{
	int i;
	int j;
	int k;
	t_sprite *tmp;

	if (l >= r || mid < l || mid > r)
		return;

	if ((r == l + 1) && (sprites[l].dist > sprites[r].dist))
	{
		swap(&sprites[l], &sprites[r]);
		return;
	}
	tmp = malloc(sizeof(t_sprite) * (r - l + 1));
	
	i = l;
	while (i < r + 1)
	{
		tmp[i - l] = sprites[i];
		++i;
	}
	i = l;
	j = 0;
	k = mid - l + 1;
	while (i <= r)
	{
		if (j > mid - l)
			sprites[i] = tmp[k++];
		else if (k > r - l)
			sprites[i] = tmp[j++];
		else
			sprites[i] = (tmp[j].dist < tmp[k].dist) ? tmp[j++] : tmp[k++];
		++i;
	}
	ft_free(tmp);
}

void sort_sprites(t_sprite *sprites, int l, int r)
{
	int mid;

	if (l >= r)
		return;
	mid = l + (r - l) / 2;
	sort_sprites(sprites, l, mid);
	sort_sprites(sprites, mid + 1, r);
	merge(sprites, l, r, mid);
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

int clear_screen(t_mlx *mlx)
{
	int x;
	int y;
	int color_floor;
	int color_ceil;

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

void	count_step_side(t_ray *ray, t_mlx *mlx)
{
	if (ray->dir.x < 0)
	{
		ray->step.x = -1;
		ray->side.x = (mlx->player.pos.x - ray->map.x) * ray->delta.x;
	}
	else
	{
		ray->step.x = 1;
		ray->side.x = (ray->map.x + 1.0 - mlx->player.pos.x) * ray->delta.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step.y = -1;
		ray->side.y = (mlx->player.pos.y - ray->map.y) * ray->delta.y;
	}
	else
	{
		ray->step.y = 1;
		ray->side.y = (ray->map.y + 1.0 - mlx->player.pos.y) * ray->delta.y;
	}
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


void	texture_walls(t_mlx *mlx, t_ray *ray, int x)
{
	t_img *texture;
	int line_h;
	t_ipos draw; 
	double step;
	double tex_pos;
	t_ipos tex;
	int y;
	draw.x = 0;
	draw.y = 0;
	
	if (ray->world_side == 'N')
		texture = mlx->texture_NO;
	else if (ray->world_side == 'S')
		texture = mlx->texture_SO;
	else if (ray->world_side == 'W')
		texture = mlx->texture_WE;
	else if (ray->world_side == 'E')
		texture = mlx->texture_EA;
	if (ray->world_side == 'S' || ray->world_side == 'N')
		ray->perp = (ray->map.x - mlx->player.pos.x + (1 - ray->step.x) / 2) / ray->dir.x;
	else
		ray->perp = (ray->map.y - mlx->player.pos.y + (1 - ray->step.y) / 2) / ray->dir.y;
	line_h = (int)(mlx->img.height / ray->perp);
	draw.x = -line_h / 2 + mlx->img.height / 2;
	if (draw.x < 0)
		draw.x = 0;
	draw.y = line_h / 2 + mlx->img.height / 2;
	if (draw.y >= mlx->img.height)
		draw.y = mlx->img.height - 1;
	if (ray->world_side == 'S' || ray->world_side == 'N')
		ray->wall_x = mlx->player.pos.y + ray->perp * ray->dir.y;
	else
		ray->wall_x = mlx->player.pos.x + ray->perp * ray->dir.x;
	ray->wall_x -= floor(ray->wall_x);
	tex.x = (int)(ray->wall_x * (double)texture->width);
	if (ray->world_side == 'S' || ray->world_side == 'W')
		tex.x = texture->width - tex.x - 1;
	step = 1.0 * texture->height / line_h;
	tex_pos = (draw.x - mlx->img.height / 2 + line_h / 2) * step;
	y = draw.x;
	while (y < draw.y)
	{
		tex.y = (int)tex_pos;
		tex_pos += step;
		mlx->img.data[y * mlx->img.width + x] = texture->data[tex.y * texture->height + tex.x];
		++y;
	}
}

void 	draw_walls(t_mlx *mlx)
{
	int x;
	int y;
	t_ray ray;

	x = 0;
	while (x < mlx->img.width)
	{
		ray.camera_x = 2 * x / (double)mlx->img.width - 1;
		ray.dir.x = mlx->player.dir.x + mlx->player.plane.x * ray.camera_x;
		ray.dir.y = mlx->player.dir.y + mlx->player.plane.y * ray.camera_x;
		ray.map.x = (int)mlx->player.pos.x;
		ray.map.y = (int)mlx->player.pos.y;
		ray.delta.x = (ray.dir.y == 0) ? 0 : ((ray.dir.x == 0) ? 1 : fabs(1 / ray.dir.x));
		ray.delta.y = (ray.dir.x == 0) ? 0 : ((ray.dir.y == 0) ? 1 : fabs(1 / ray.dir.y));
		count_step_side(&ray, mlx);
		dda_alg(mlx, &ray);
		texture_walls(mlx, &ray, x);
		mlx->perp_buff[x] = ray.perp;
		++x;
	}
	
}

void	floor_line(t_mlx *mlx, t_floor *floor, int y)
{
	floor->dir0.x = mlx->player.dir.x - mlx->player.plane.x;
	floor->dir1.x = mlx->player.dir.x + mlx->player.plane.x;
	floor->dir0.y = mlx->player.dir.y - mlx->player.plane.y;
	floor->dir1.y = mlx->player.dir.y + mlx->player.plane.y;
	floor->p = y - mlx->img.height / 2;
	floor->pos_z = 0.5 * mlx->img.height;
	floor->row_dist = floor->pos_z / floor->p;
	floor->step.x = floor->row_dist * (floor->dir1.x - floor->dir0.x) / mlx->img.width;
	floor->step.y = floor->row_dist * (floor->dir1.y - floor->dir0.y) / mlx->img.width;
	floor->pos.x = mlx->player.pos.x + floor->row_dist * floor->dir0.x;
	floor->pos.y = mlx->player.pos.y + floor->row_dist * floor->dir0.y;
}
void	draw_floor(t_mlx *mlx, t_ipos *range)
{
	t_floor floor;

	int x;
	int y;
	t_ipos tex_pos;
	t_img *texture;

	y = range->x;
	while (y < range->y)
	{
		floor_line(mlx, &floor, y);
		x = 0;
		texture = mlx->texture_floor;
		while (x < mlx->img.width)
		{
			floor.cell.x = (int)floor.pos.x;
			floor.cell.y = (int)floor.pos.y;
			tex_pos.x = (int)(texture->width * (floor.pos.x - floor.cell.x)) & texture->width - 1;
			tex_pos.y = (int)(texture->height * (floor.pos.y - floor.cell.y)) & texture->height - 1;
			floor.pos.x += floor.step.x;
			floor.pos.y += floor.step.y;
			int color = texture->data[tex_pos.y * texture->width + tex_pos.x];
			color = (color >> 1) & 8355711;
			mlx->img.data[y * mlx->img.width + x] = color;
			++x;
		}
		++y;
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

void	get_sprites_pos(t_mlx *mlx, t_sprite *sprites)
{
	int i;
	int j;
	int n;

	i = 0;
	n = 0;
	while (i < mlx->map->height)
	{
		j = 0;
		while (j < mlx->map->width)
		{
			if (mlx->map->map_ptr[i][j] == '2')
			{
				sprites[n].x = i + 0.5;
				sprites[n].y = j + 0.5;
				++n;
			}
			++j;
		}
		++i;
	}
}
void	draw_sprites(t_mlx *mlx, t_sprite *sprites, int num_sprites)
{
	
	t_sprite_vars sv;
	int n;
	int k;
	int d;
	int color;

	
	get_sprites_pos(mlx, sprites);
	n = 0;
	while (n < num_sprites)
	{
		sprites[n].dist = (mlx->player.pos.x - (double)sprites[n].x) * (mlx->player.pos.x - (double)sprites[n].x) + (mlx->player.pos.y - (double)sprites[n].y) * (mlx->player.pos.y - (double)sprites[n].y);
		++n;
	}
	sort_sprites(sprites, 0, num_sprites - 1);
	n = num_sprites;
	while (n >= 0)
	{
		sv.pos.x = sprites[n].x - mlx->player.pos.x;
		sv.pos.y = sprites[n].y - mlx->player.pos.y;
		sv.inv_det = 1.0 / (mlx->player.plane.x * mlx->player.dir.y - mlx->player.plane.y * mlx->player.dir.x);
		sv.transform.x = sv.inv_det * (mlx->player.dir.y * sv.pos.x - mlx->player.dir.x * sv.pos.y);
		sv.transform.y = sv.inv_det * ((-1) * mlx->player.plane.y * sv.pos.x + mlx->player.plane.x * sv.pos.y);
		sv.screen_x = (int)((mlx->img.width / 2) * (1 + sv.transform.x / sv.transform.y));
		sv.height = abs((int)(mlx->img.height / sv.transform.y));
		sv.draw_y.x = -sv.height / 2 + mlx->img.height / 2;
		if (sv.draw_y.x < 0)
			sv.draw_y.x = 0;
		sv.draw_y.y = sv.height / 2 + mlx->img.height / 2;
		if (sv.draw_y.y >= mlx->img.height)
			sv.draw_y.y = mlx->img.height - 1;
		sv.width = abs((int)(mlx->img.height / sv.transform.y));
		sv.draw_x.x = -sv.width / 2 + sv.screen_x;
		if (sv.draw_x.x < 0)
			sv.draw_x.x = 0;
		sv.draw_x.y = sv.width / 2 + sv.screen_x;
		if (sv.draw_x.y >= mlx->img.width)
			sv.draw_x.y = mlx->img.width - 1;
		sv.stripe = sv.draw_x.x;
		while (sv.stripe < sv.draw_x.y)
		{
			sv.tex.x = (int)(256 * (sv.stripe - (-sv.width / 2 + sv.screen_x)) * mlx->texture_sprite->width / sv.width) / 256;
			if (sv.transform.y > 0 && sv.stripe > 0 && sv.stripe < mlx->img.width && sv.transform.y < mlx->perp_buff[sv.stripe])
			{
				k = sv.draw_y.x;
				while (k < sv.draw_y.y)
				{
					d = k * 256 - mlx->img.height * 128 + sv.height * 128;
					sv.tex.y = ((d * mlx->texture_sprite->height) / sv.height) / 256;
					color = mlx->texture_sprite->data[mlx->texture_sprite->width * sv.tex.y + sv.tex.x];
					if (color != 0)
						mlx->img.data[k * mlx->img.width + sv.stripe] = color;
					++k;
				}
			}
			++sv.stripe;
		}
		--n;
	}

}

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
		texture->data = (int *)mlx_get_data_addr(texture->img_ptr, &texture->bpp,
												&texture->size_l, &texture->endian);
	}
	if (errno)
	{
		perror("Error\nOpen texture");
		//printf("0\n");
		return (STANDART_ERROR);
	}
	return (0);
}



int		set_textures(t_settings *settings, t_mlx *mlx)
{
	int code;
	
	code = 0;
	if ((code = set_texture(mlx, mlx->texture_SO, settings->south_tex_path)) < 0)
		return (code);
	if ((code = set_texture(mlx, mlx->texture_WE, settings->west_tex_path)) < 0)
		return (code);
	if ((code = set_texture(mlx, mlx->texture_NO, settings->north_tex_path)) < 0)
		return (code);
	if ((code = set_texture(mlx, mlx->texture_EA, settings->east_tex_path)) < 0)
		return (code);
	if ((code = set_texture(mlx, mlx->texture_sprite, settings->sprite_path)) < 0)
		return (code);
	if (settings->ceil_tex_path)
	{
		if ((code = set_texture(mlx, mlx->texture_ceil, settings->ceil_tex_path)) < 0)
			return (code);
	}
	if (settings->floor_tex_path)
	{
		if ((code = set_texture(mlx, mlx->texture_floor, settings->floor_tex_path)) < 0)
		{
			//printf("2\n");
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
	mlx->texture_ceil = malloc(sizeof(t_img));
	mlx->texture_floor = malloc(sizeof(t_img));
	mlx->texture_NO = malloc(sizeof(t_img));
	mlx->texture_SO = malloc(sizeof(t_img));
	mlx->texture_WE = malloc(sizeof(t_img));
	mlx->texture_EA = malloc(sizeof(t_img));
	mlx->texture_sprite = malloc(sizeof(t_img));

	if ((code = set_textures(settings, mlx)) < 0)
		return (code);
	mlx->player.pos.x = (double)settings->player.pos.x;
	mlx->player.pos.y = (double)settings->player.pos.y;
	get_direction(settings->player.dir, &mlx->player.dir);
	get_plane(settings->player.dir, &mlx->player.plane, 0.66);
	mlx->map = &settings->map;
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
	mlx->num_sprites = count_sprites(mlx);
	mlx->sprites = malloc(sizeof(t_sprite) * mlx->num_sprites);
	
	return (0);
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
	draw_floor(mlx, &range);
	draw_walls(mlx);
	draw_sprites(mlx, mlx->sprites, mlx->num_sprites);
	
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
	draw(mlx);
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
	return (0);
}

void	mlx_free(t_mlx *mlx)
{
	ft_free(&mlx->texture_SO);
	ft_free(&mlx->texture_NO);
	ft_free(&mlx->texture_WE);
	ft_free(&mlx->texture_EA);
	ft_free(&mlx->texture_sprite);
	ft_free(&mlx->texture_floor);
	ft_free(&mlx->texture_ceil);
	ft_free(mlx->sprites);
	ft_free(mlx->perp_buff);
}



int		cube_init(char *file, int save)
{
	t_mlx mlx;
	t_settings *settings;

	settings = (t_settings *)malloc(sizeof(t_settings));
	init_settings(settings);
	settings->file = file;
	settings->save = save;
	//mlx = malloc(sizeof(t_mlx));
	cube(&mlx, settings);
	parser_free(settings);
	mlx_free(&mlx);
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
		{
			save = 1;
			printf("saved\n");
		}
		cube_init(av[1], save);
	}
	
	
	
	return (0);
}