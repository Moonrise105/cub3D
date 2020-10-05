#include "cub.h"

static void	get_sprites_pos(t_mlx *mlx, t_sprite *sprites)
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

static int	get_sorted_sprites(t_mlx *mlx, t_sprite *sprites, int num_sprites)
{
	int n;

	get_sprites_pos(mlx, sprites);
	n = 0;
	while (n < num_sprites)
	{
		sprites[n].dist = (mlx->player.pos.x - (double)sprites[n].x) * 
		(mlx->player.pos.x - (double)sprites[n].x) + 
		(mlx->player.pos.y - (double)sprites[n].y) * (mlx->player.pos.y - (double)sprites[n].y);
		++n;
	}
	return (sort_sprites(sprites, 0, num_sprites - 1));
}

static void calculate_vars(t_sprite_vars *sv, t_mlx *mlx, t_sprite *sprites, int n)
{
	sv->pos.x = sprites[n].x - mlx->player.pos.x;
	sv->pos.y = sprites[n].y - mlx->player.pos.y;
	sv->inv_det = 1.0 / (mlx->player.plane.x * mlx->player.dir.y - mlx->player.plane.y * mlx->player.dir.x);
	sv->transform.x = sv->inv_det * (mlx->player.dir.y * sv->pos.x - mlx->player.dir.x * sv->pos.y);
	sv->transform.y = sv->inv_det * ((-1) * mlx->player.plane.y * sv->pos.x + mlx->player.plane.x * sv->pos.y);
	sv->screen_x = (int)((mlx->img.width / 2) * (1 + sv->transform.x / sv->transform.y));
	sv->height = abs((int)(mlx->img.height / sv->transform.y));
	sv->draw_y.x = -sv->height / 2 + mlx->img.height / 2;
	if (sv->draw_y.x < 0)
		sv->draw_y.x = 0;
	sv->draw_y.y = sv->height / 2 + mlx->img.height / 2;
	if (sv->draw_y.y >= mlx->img.height)
		sv->draw_y.y = mlx->img.height - 1;
	sv->width = abs((int)(mlx->img.height / sv->transform.y));
	sv->draw_x.x = -sv->width / 2 + sv->screen_x;
	if (sv->draw_x.x < 0)
		sv->draw_x.x = 0;
	sv->draw_x.y = sv->width / 2 + sv->screen_x;
	if (sv->draw_x.y >= mlx->img.width)
		sv->draw_x.y = mlx->img.width - 1;
	sv->stripe = sv->draw_x.x;
}

static void draw_tex_sprites(t_sprite_vars *sv, t_mlx *mlx)
{
	int color;
	int k;
	int d;

	sv->tex.x = (int)(256 * (sv->stripe - (-sv->width / 2 + sv->screen_x)) *
					mlx->tex_s->width / sv->width) / 256;
	if (sv->transform.y > 0 && sv->stripe > 0 && sv->stripe < mlx->img.width
		&& sv->transform.y < mlx->perp_buff[sv->stripe])
	{
		k = sv->draw_y.x;
		while (k < sv->draw_y.y)
		{
			d = k * 256 - mlx->img.height * 128 + sv->height * 128;
			sv->tex.y = ((d * mlx->tex_s->height) / sv->height) / 256;
			color = mlx->tex_s->data[mlx->tex_s->width * sv->tex.y + sv->tex.x];
			if (color != 0)
				mlx->img.data[k * mlx->img.width + sv->stripe] = color;
			++k;
		}
	}
}

int 	draw_sprites(t_mlx *mlx, t_sprite *sprites, int num_sprites)
{	
	t_sprite_vars sv;
	int n;

	if (get_sorted_sprites(mlx, sprites, num_sprites) < 0)
		return (MALLOC_ERROR);
	n = num_sprites;
	while (n >= 0)
	{
		calculate_vars(&sv, mlx, sprites, n);
		while (sv.stripe < sv.draw_x.y)
		{
			draw_tex_sprites(&sv, mlx);
			++sv.stripe;
		}
		--n;
	}
	return (0);
}