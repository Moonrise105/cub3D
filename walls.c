#include "cub.h"

static void	count_step_side(t_ray *ray, t_mlx *mlx)
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

static t_img *get_texture(t_ray *ray, t_mlx *mlx)
{
	t_img *texture;

	if (ray->world_side == 'N')
		texture = mlx->tex_NO;
	else if (ray->world_side == 'S')
		texture = mlx->tex_SO;
	else if (ray->world_side == 'W')
		texture = mlx->tex_WE;
	else if (ray->world_side == 'E')
		texture = mlx->tex_EA;
	return (texture);
}

static void draw_tex_walls(t_ray *ray, t_mlx *mlx, int x, t_ipos draw)
{
	t_ipos tex;
	double step;
	double tex_pos;
	t_img *texture;

	texture = get_texture(ray, mlx);
	tex.x = (int)(ray->wall_x * (double)texture->width);
	if (ray->world_side == 'S' || ray->world_side == 'W')
		tex.x = texture->width - tex.x - 1;
	step = 1.0 * texture->height / ray->line_h;
	tex_pos = (draw.x - mlx->img.height / 2 + ray->line_h / 2) * step;
	while (draw.x < draw.y)
	{
		tex.y = (int)tex_pos;
		tex_pos += step;
		mlx->img.data[draw.x * mlx->img.width + x] = texture->data[tex.y * texture->height + tex.x];
		++draw.x;
	}
}

static void	texture_walls(t_mlx *mlx, t_ray *ray, int x)
{
	t_ipos draw; 
	t_ipos tex;
	int y;

	draw.x = 0;
	draw.y = 0;
	if (ray->world_side == 'S' || ray->world_side == 'N')
		ray->perp = (ray->map.x - mlx->player.pos.x + (1 - ray->step.x) / 2) / ray->dir.x;
	else
		ray->perp = (ray->map.y - mlx->player.pos.y + (1 - ray->step.y) / 2) / ray->dir.y;
	ray->line_h = (int)(mlx->img.height / ray->perp);
	draw.x = -ray->line_h / 2 + mlx->img.height / 2;
	if (draw.x < 0)
		draw.x = 0;
	draw.y = ray->line_h / 2 + mlx->img.height / 2;
	if (draw.y >= mlx->img.height)
		draw.y = mlx->img.height - 1;
	if (ray->world_side == 'S' || ray->world_side == 'N')
		ray->wall_x = mlx->player.pos.y + ray->perp * ray->dir.y;
	else
		ray->wall_x = mlx->player.pos.x + ray->perp * ray->dir.x;
	ray->wall_x -= floor(ray->wall_x);
	draw_tex_walls(ray, mlx, x, draw);
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