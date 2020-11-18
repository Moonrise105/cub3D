/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 16:36:39 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/22 21:28:16 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H
# include <fcntl.h>
# include <stdlib.h>
# include "./mlx/mlx.h"
# include "./libft/libft.h"
# include <errno.h>
# include <math.h>
# define MAX_WIDTH 2560
# define MAX_HEIGHT 1440
# define VELOCITY 0.31
# define ROTATE 0.2
# define COLOR_ERROR -2
# define RESOLUTION_ERROR -1
# define MAP_ERROR -3
# define ID_ERROR -4
# define STANDART_ERROR -6
# define MALLOC_ERROR -5
# define FORMAT_ERROR -7
# define ARG_ERROR -8
# define BUFFER_SIZE 32

typedef struct		s_map
{
	char			**map_ptr;
	int				width;
	int				height;
}					t_map;

typedef struct		s_ipos
{
	int				x;
	int				y;
}					t_ipos;

typedef struct		s_fpos
{
	double			x;
	double			y;
}					t_fpos;

typedef struct		s_color_rgb
{
	int				r;
	int				g;
	int				b;
}					t_color_rgb;

typedef struct		s_sprite
{
	double			x;
	double			y;
	double			dist;
}					t_sprite;

typedef struct		s_player
{
	t_fpos			pos;
	t_fpos			dir;
	t_fpos			plane;
}					t_player;

typedef struct		s_sprite_vars
{
	t_fpos			pos;
	double			inv_det;
	t_fpos			transform;
	int				screen_x;
	int				height;
	t_ipos			draw_x;
	t_ipos			draw_y;
	int				width;
	int				stripe;
	t_ipos			tex;
}					t_sprite_vars;

typedef struct		s_ray
{
	double			camera_x;
	t_fpos			dir;
	t_fpos			delta;
	t_fpos			side;
	double			perp;
	t_ipos			map;
	t_ipos			step;
	double			wall_x;
	char			world_side;
	int				line_h;
}					t_ray;

typedef struct		s_img
{
	void			*img_ptr;
	int				*data;
	int				size_l;
	int				bpp;
	int				endian;
	int				width;
	int				height;
}					t_img;

typedef struct		s_mlx
{
	void			*mlx_ptr;
	void			*win;
	t_img			img;
	t_img			*tex_so;
	t_img			*tex_we;
	t_img			*tex_ea;
	t_img			*tex_no;
	t_img			*tex_s;
	t_img			*tex_f;
	t_img			*tex_c;
	t_player		player;
	t_map			*map;
	double			*perp_buff;
	t_color_rgb		*ceil;
	t_color_rgb		*floor;
	t_sprite		*sprites;
	int				num_sprites;
}					t_mlx;

typedef struct		s_floor
{
	t_fpos			dir0;
	t_fpos			dir1;
	t_fpos			step;
	t_fpos			pos;
	t_ipos			cell;
	double			pos_z;
	double			row_dist;
	int				p;
}					t_floor;

typedef struct		s_player_map
{
	t_ipos			pos;
	char			dir;
}					t_player_map;

typedef struct		s_settings
{
	int				save;
	char			*file;
	int				max_res_x;
	int				max_res_y;
	t_color_rgb		color_floor;
	t_color_rgb		color_ceil;
	int				resolution_x;
	int				resolution_y;
	char			*north_tex_path;
	char			*south_tex_path;
	char			*west_tex_path;
	char			*east_tex_path;
	char			*floor_tex_path;
	char			*ceil_tex_path;
	char			*sprite_path;
	t_map			map;
	t_sprite		sprites;
	t_player_map	player;
}					t_settings;

int					ft_max(int a, int b);
void				print_error(int code);
int					ft_mini_atoi(char **str);
int					check_line(char *line, t_settings *settings);
int					get_max_line_size(t_list *head);
void				mem_set_char(char **ptr, char c, int size);
int					is_valid_map_symbol(char c);
void				del_str(void *ptr);
void				array2d_clear(char **array, int height);
int					parse_map(int fd, char *line, t_settings *settings);
int					check_settings(t_settings *settings);
int					parse_file(t_settings *settings);
int					parser(t_settings *settings);
void				init_settings(t_settings *settings);
int					check_format(char *file, char *format);
void				parser_free(t_settings *settings);
void				ft_free(void *ptr);
int					bitmap(t_mlx *mlx);
int					draw_sprites(t_mlx *mlx,
					t_sprite *sprites, int num_sprites);
void				draw_walls(t_mlx *mlx);
void				draw_floor(t_mlx *mlx, t_ipos *range);
void				dda_alg(t_mlx *mlx, t_ray *ray);
int					sort_sprites(t_sprite *sprites, int l, int r);
int					set_texture(t_mlx *mlx, t_img *texture, char *path);
int					set_textures(t_settings *settings, t_mlx *mlx);
void				get_plane(char c, t_fpos *plane, double pov);
void				get_direction(char c, t_fpos *dir);
int					set_settings(t_settings *settings, t_mlx *mlx);
int					mlx_alloc(t_mlx *mlx, t_settings *settings);
int					rgb_to_int(t_color_rgb *color_rgb);
void				mlx_free(t_mlx *mlx);
void				mlx_struct_init(t_mlx *mlx);
int					count_sprites(t_mlx *mlx);
int					draw(t_mlx *mlx);
int					canvas_init(t_mlx *mlx, t_settings *settings);
void				start_game(t_mlx *mlx, int save);
void				reset_image(t_mlx *mlx);
int					win_close(t_mlx *mlx);
void				get_ray_delta(t_ray *ray);
void				skip_n(char **line, int fd);
int					get_next_line(int fd, char **line);
#endif
