#include "cub.h"
#include "get_next_line.h"

static	int	check_map_element(t_settings *settings, int i, int j)
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
	return (0);
}

static int		check_map(t_settings *settings)
{
	int		i;
	int		j;
	int		code;

	i = 0;
	while (i < settings->map.height)
	{
		j = 0;
		while (j < settings->map.width)
		{
			code = check_map_element(settings, i, j);
			if (code < 0)
				return (code);
			++j;
		}
		++i;
	}
	if (!settings->player.dir)
		return (MAP_ERROR);
	return (0);
}

static void fill_line(t_settings *settings, int i, t_list *head)
{
	int		j;
	int		line_size;

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
}

static int		make_map(t_list *head, t_settings *settings)
{
	int		i;
	int		map_width;

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
		fill_line(settings, i, head);
		head = head->next;
		++i;
	}
	return (check_map(settings));
}

int		parse_map(int fd, char *line, t_settings *settings)
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
