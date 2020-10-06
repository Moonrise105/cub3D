/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_00.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:23:17 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/06 17:23:32 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int			ft_max(int a, int b)
{
	if (a >= b)
		return (a);
	return (b);
}

void		ft_free(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void		print_error(int code)
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

int			ft_mini_atoi(char **str)
{
	int		res;

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

int			get_max_line_size(t_list *head)
{
	int		res;
	int		size;

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
