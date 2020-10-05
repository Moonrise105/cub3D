#include "cub.h"

static int			parse_path(char **path, char *line)
{
	while (*line && *line == ' ')
		line++;
	*path = ft_strdup(line);
	return (0);
}

static int			parse_res(int *res_x, int *res_y, char *line)
{
	*res_x = ft_mini_atoi(&line);
	*res_y = ft_mini_atoi(&line);
	if (*res_x == 0 || *res_y == 0 || *line != '\0')
		return (RESOLUTION_ERROR);
	return (0);
}

static int			parse_color(t_color_rgb *color, char *line)
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

static int		check_id(char *id, t_settings *settings, char *line)
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

int			check_line(char *line, t_settings *settings)
{
	int		i;
	char	*id;

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