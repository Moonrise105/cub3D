/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 00:38:40 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/09 16:00:23 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./get_next_line.h"

void		ft_strclr(char *s)
{
	int	i;

	i = 0;
	while (s && *(s + i))
	{
		*(s + i) = '\0';
		i++;
	}
}

void		ft_free_c(char **p)
{
	if (*p)
	{
		free(*p);
		*p = NULL;
	}
}

int			check_result(int *read_len, char **read_buff, char **n_pos)
{
	if ((read_len && *read_len) || ft_strlen(*read_buff) || *n_pos)
		return (1);
	else
	{
		if (*read_buff)
		{
			free(*read_buff);
			*read_buff = NULL;
		}
		return (0);
	}
}

char		*check_read_buff(char *read_buff, char **line)
{
	char *p;

	p = NULL;
	if (read_buff && ft_strlen(read_buff))
	{
		if ((p = ft_strchr(read_buff, '\n')))
		{
			*p = '\0';
			*line = ft_strdup(read_buff);
			++p;
			ft_strlcpy(read_buff, p, ft_strlen(p) + 1);
		}
		else
		{
			*line = ft_strdup(read_buff);
			ft_strclr(read_buff);
		}
	}
	else
	{
		*line = ft_strdup("\0");
	}
	return (p);
}

int			get_next_line(int fd, char **line)
{
	char		buff[BUFFER_SIZE + 1];
	int			read_len;
	static char *read_buff;
	char		*n_pos;
	char		*tmp;

	read_len = 0;
	if (fd < 0 || !line || (read(fd, buff, 0)) < 0 || BUFFER_SIZE < 1)
		return (-1);
	n_pos = check_read_buff(read_buff, line);
	while (!n_pos && (read_len = read(fd, buff, BUFFER_SIZE)))
	{
		buff[read_len] = '\0';
		if ((n_pos = ft_strchr(buff, '\n')))
		{
			*n_pos = '\0';
			ft_free_c(&read_buff);
			read_buff = ft_strdup(++n_pos);
		}
		tmp = *line;
		*line = ft_strjoin(*line, buff);
		free(tmp);
	}
	return (check_result(&read_len, &read_buff, &n_pos));
}
