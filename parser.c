/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:48:48 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/22 17:11:48 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub.h"

int		parse_file(t_settings *settings)
{
	int		fd;
	char	*line;
	int		code;

	errno = 0;
	fd = open(settings->file, O_RDONLY);
	if (errno)
	{
		perror("Error\nOpen");
		return (STANDART_ERROR);
	}
	while (get_next_line(fd, &line) > 0)
	{
		if (check_settings(settings))
			return (parse_map(fd, line, settings));
		code = check_line(line, settings);
		ft_free(line);
		if (code < 0)
			return (code);
	}
	close(fd);
	if (!check_settings(settings) || !settings->map.map_ptr)
		return (MAP_ERROR);
	return (code);
}

int		parser(t_settings *settings)
{
	int code;

	code = parse_file(settings);
	if (code < 0)
		print_error(code);
	return (code);
}

void	init_settings(t_settings *settings)
{
	settings->resolution_x = 0;
	settings->resolution_y = 0;
	settings->color_floor.r = -1;
	settings->color_floor.g = -1;
	settings->color_floor.b = -1;
	settings->color_ceil.r = -1;
	settings->color_ceil.g = -1;
	settings->color_ceil.b = -1;
	settings->east_tex_path = NULL;
	settings->north_tex_path = NULL;
	settings->west_tex_path = NULL;
	settings->south_tex_path = NULL;
	settings->sprite_path = NULL;
	settings->player.dir = '\0';
	settings->floor_tex_path = NULL;
	settings->ceil_tex_path = NULL;
}

void	parser_free(t_settings *settings)
{
	ft_free(settings->north_tex_path);
	ft_free(settings->south_tex_path);
	ft_free(settings->west_tex_path);
	ft_free(settings->east_tex_path);
	ft_free(settings->floor_tex_path);
	ft_free(settings->ceil_tex_path);
	ft_free(settings->sprite_path);
	array2d_clear(settings->map.map_ptr,
	settings->map.height);
	ft_free(settings);
}
