#include "libft/libft.h"
#include "./cub.h"
#include <fcntl.h>
#include "./get_next_line.h"





#include <stdio.h>

int ft_max(int a, int b)
{
	if (a >= b)
		return (a);
	return (b);
}

void	ft_free(void *ptr) //good
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	print_error(int code) //good
{
	if (code != STANDART_ERROR)
		ft_putstr_fd("Error\n", 2);
	if (code == COLOR_ERROR)
		ft_putstr_fd("Color parse error\n", 2);
	else if (code == RESOLUTION_ERROR)
		ft_putstr_fd("Resolution parse error\n", 2);
	else if (code == ID_ERROR)
	 	ft_putstr_fd("ID parse error\n", 2);
	else if (code == MALLOC_ERROR)
		ft_putstr_fd("Malloc error\n", 2);
	else if (code == MAP_ERROR)
		ft_putstr_fd("Map error\n", 2);
	else if (code == FORMAT_ERROR)
		ft_putstr_fd("Format error\n", 2);

	return ;
}

int		ft_mini_atoi(char **str) //good
{
	int res;

	res = -1;
	while (**str && **str == ' ')
		(*str)++;
	while (**str && ft_isdigit(**str))
	{
		if (res == -1)
			res = 0;
		res = res * 10 + (**str - 48);
		(*str)++;
	}
	return (res);
}


int		parse_path(char **path, char *line) //need to return path
{
	while (*line && *line == ' ')
		line++;
	*path = ft_strdup(line);
	return (0);
}

int		parse_res(int *res_x, int *res_y, char *line)
{
	*res_x = ft_mini_atoi(&line);
	*res_y = ft_mini_atoi(&line);
	if (*res_x == 0 || *res_y == 0 || *line != '\0')
		return (RESOLUTION_ERROR);
	return (0);
}

int		parse_color(t_color_rgb *color, char *line)
{
	color->r = ft_mini_atoi(&line);
	if (!(*line) || *line != ',' || color->r < 0 || color->r > 255)
		return (COLOR_ERROR);
	line++;
	color->g = ft_mini_atoi(&line);
	if (!(*line) || *line != ',' || color->g < 0 || color->g > 255)
		return (COLOR_ERROR);
	line++;
	color->b = ft_mini_atoi(&line);
	if (*line != '\0' || color->b < 0 || color->b > 255)
		return (COLOR_ERROR);
	return (0);
}

int		check_id(char *id, t_settings *settings, char *line) //free good
{
	if (ft_strncmp(id, "R", 2) == 0)
		return (parse_res(&settings->resolution_x, &settings->resolution_y, 
					++line));
	else if (ft_strncmp(id, "NO", 3) == 0)
		return (parse_path(&settings->north_tex_path, line += 2));
	else if (ft_strncmp(id, "SO", 3) == 0)
		return (parse_path(&settings->south_tex_path, line += 2));
	else if (ft_strncmp(id, "WE", 3) == 0)
		return (parse_path(&settings->west_tex_path, line += 2));
	else if (ft_strncmp(id, "EA", 3) == 0)
		return (parse_path(&settings->east_tex_path, line += 2));
	else if (ft_strncmp(id, "FT", 3) == 0)
		return (parse_path(&settings->floor_tex_path, line += 2));
	else if (ft_strncmp(id, "CT", 3) == 0)
		return (parse_path(&settings->ceil_tex_path, line += 2));
	else if (ft_strncmp(id, "S", 3) == 0)
		return (parse_path(&settings->sprite_path, ++line));
	else if (ft_strncmp(id, "F", 3) == 0)
		return (parse_color(&settings->color_floor, ++line));
	else if (ft_strncmp(id, "C", 3) == 0)
		return (parse_color(&settings->color_ceil, ++line));
	return (ID_ERROR);
}

int		check_line(char *line, t_settings *settings) //free good
{
	int i;
	char *id;

	if (ft_strlen(line) == 0)
		return (0);
	i = 0;
	while (line[i] && line[i] != ' ')
		++i;
	if (i <= 2)
	{
		if (!(id = (char *)malloc(sizeof(char) * i + 1)))
			return (MALLOC_ERROR);
		id[i] = '\0';
		while (i > 0)
		{
			--i;
			id[i] = line[i]; 
		}
		i = check_id(id, settings, line);
		ft_free(id);
		return (i);
	}
	
	return (ID_ERROR);
}

int		get_max_line_size(t_list *head) //good
{
	int res;
	int size;

	res = 0;
	while (head)
	{
		size = ft_strlen(head->content);
		if (size > res)
			res = size;
		head = head->next;
	}
	return (res);
}

void	mem_set_char(char **ptr, char c, int size) //good
{
	int i;

	i = 0;
	if (ptr && *ptr)
	{
		while (i < size)
		{
			(*ptr)[i] = c;
			++i;
		}
	}
}

int		is_valid_map_symbol(char c) //good
{
	char *objects = "012 ";
	char *player = "NWSE";
	
	if (ft_strchr(objects, c))
		return (1);
	else if (ft_strchr(player, c))
		return (2);
	return (0);
}

int		check_map(t_settings *settings) //free good
{
	int i;
	int j;

	i = 0;
	while (i < settings->map.height)
	{
		j = 0;
		while (j < settings->map.width)
		{
			if (i == 0 && settings->map.map_ptr[i][j] != '1')
				return (MAP_ERROR);
			if ((i == settings->map.height - 1) && settings->map.map_ptr[i][j] != '1')
				return (MAP_ERROR);
			if (j == 0 && settings->map.map_ptr[i][j] != '1')
				return (MAP_ERROR);
			if ((j == settings->map.width - 1) && settings->map.map_ptr[i][j] != '1')
				return (MAP_ERROR);
			if (!is_valid_map_symbol(settings->map.map_ptr[i][j]))
				return (MAP_ERROR);
			if (is_valid_map_symbol(settings->map.map_ptr[i][j]) == 2)
			{
				if (!settings->player.dir)
				{
					settings->player.dir = settings->map.map_ptr[i][j];
					settings->player.pos.x = i;
					settings->player.pos.y = j;
				}
				else
					return (MAP_ERROR);
			}
			++j;
		}
		++i;
	}
	if (!settings->player.dir)
		return (MAP_ERROR);
	return (0);
}

int		make_map(t_list *head, t_settings *settings) //free good
{
	int i;
	int map_width;
	int line_size;
	int j;

	if (!(settings->map.map_ptr = malloc(sizeof(char *) * settings->map.height)))
		return (MALLOC_ERROR);
	settings->map.width = get_max_line_size(head);
	i = 0;
	while (i < settings->map.height)
	{
		if(!(settings->map.map_ptr[i] = malloc(sizeof(char) * settings->map.width)))
		{
			while (--i >= 0)
				ft_free(settings->map.map_ptr[i]);
			ft_free(settings->map.map_ptr);
			return (MALLOC_ERROR);
		}
		mem_set_char(&settings->map.map_ptr[i], '1', settings->map.width);
		j = 0;
		line_size = ft_strlen(head->content);
		while (j < line_size)
		{
			if (head->content[j] == ' ')
				settings->map.map_ptr[i][j] = '1';
			else
				settings->map.map_ptr[i][j] = (head->content[j]);
			++j;
		}
		head = head->next;
		++i;
	}
	return (check_map(settings));
}

void	array2D_clear(char **array, int width, int height) //good
{
	int i;

	i = 0;
	while (i < height)
	{
		ft_free(array[i]);	
		++i;
	}
	ft_free(array);
}

int		parse_map(int fd, char *line, t_settings *settings) //good
{
	t_list *head;
	int code;

	if (ft_strlen(line) == 0)
		return (MAP_ERROR);
	head = ft_lstnew(line);
	if (!head)
		return (MALLOC_ERROR);
	while (get_next_line(fd, &line) > 0)
	{
		if (ft_strlen(line) == 0)
		{
			ft_lstclear(&head, ft_free);
			ft_free(line);
			return (MAP_ERROR);
		}
		ft_lstadd_back(&head, ft_lstnew(line));
	}
	ft_lstadd_back(&head, ft_lstnew(line));
	settings->map.height = ft_lstsize(head);
	code = make_map(head, settings);
	ft_lstclear(&head, ft_free);
	ft_free(head);
	return (code);
}

int		check_settings(t_settings *settings)
{
	if (((settings->color_ceil.r >= 0 && settings->color_ceil.g >= 0 &&
		settings->color_ceil.b >= 0) || settings->ceil_tex_path) &&
		((settings->color_floor.r >= 0 && settings->color_floor.g >= 0 &&
		settings->color_floor.b >= 0) || settings->floor_tex_path) &&
		settings->east_tex_path && settings->north_tex_path &&
		settings->south_tex_path && settings->west_tex_path &&
		settings->sprite_path && settings->resolution_x > 0 &&
		settings->resolution_y > 0)
		return (1);
	return (0);	
}

int		parse_file(t_settings *settings)
{
	int fd;
	char *line;
	int code;
	errno = 0;

	fd = open(settings->file, O_RDONLY);
	if (fd)
	{
		while (get_next_line(fd, &line) > 0)
		{
			if (check_settings(settings))
			{
				code = parse_map(fd, line, settings);
				// if (settings->map.height <= 2)
				// 	code = MAP_ERROR;
				return (code);
			}
			else
			{
				code = check_line(line, settings);
				//ft_free(line);
			}
			if (code < 0)
			{
				ft_free(line);
				return (code);
			}
			ft_free(line);	
		}
		close(fd);
	}
	if (errno)
	{
		perror("Error\nOpen");
		return (STANDART_ERROR);
	}
	if (!check_settings(settings) || !settings->map.map_ptr)
		return (MAP_ERROR);
	
	return (code);
}

int		check_format(char *file, char *format) //good
{
	char *p;

	p = ft_strrchr(file, '.');
	if (p && ft_strncmp(p, format, ft_max(ft_strlen(format), ft_strlen(p))) == 0)
		return (1);
	return (0);
}


int		parser(t_settings *settings)
{
	int code;

	
	code = parse_file(settings);
	if (code < 0)
	{
		print_error(code);
		//array2D_clear(settings->map.map_ptr, settings->map.width, settings->map.height);

	}
		
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
	printf("free\n");
	ft_free(settings->north_tex_path);
	ft_free(settings->south_tex_path);
	ft_free(settings->west_tex_path);
	ft_free(settings->east_tex_path);
	ft_free(settings->floor_tex_path);
	ft_free(settings->ceil_tex_path);
	ft_free(settings->sprite_path);
	array2D_clear(settings->map.map_ptr, settings->map.width, settings->map.height);
	ft_free(settings);
}

// int main()
// {
// 	t_settings *settings;

// 	settings = malloc(sizeof(t_settings));
// 	init_settings(settings);
// 	settings->file = "file.cub";
// 	parser(settings);
// 	//printf("%s\n", settings->map.map_ptr[1]);

	
// 	// while (1)
// 	// {
// 	// 	1 == 1;
// 	// }
// 	return (0);
// }