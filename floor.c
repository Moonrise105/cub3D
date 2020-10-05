#include "cub.h"

static void	floor_line(t_mlx *mlx, t_floor *floor, int y)
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

	y = range->x;
	while (y < range->y)
	{
		floor_line(mlx, &floor, y);
		x = 0;
		while (x < mlx->img.width)
		{
			floor.cell.x = (int)floor.pos.x;
			floor.cell.y = (int)floor.pos.y;
			tex_pos.x = (int)(mlx->tex_f->width * (floor.pos.x - floor.cell.x))
			& mlx->tex_f->width - 1;
			tex_pos.y = (int)(mlx->tex_f->height * (floor.pos.y - floor.cell.y))
			& mlx->tex_f->height - 1;
			floor.pos.x += floor.step.x;
			floor.pos.y += floor.step.y;
			mlx->img.data[y * mlx->img.width + x] = 
			mlx->tex_f->data[tex_pos.y * mlx->tex_f->width + tex_pos.x];
			++x;
		}
		++y;
	}
}