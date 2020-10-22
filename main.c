/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:59:14 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/22 17:09:43 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub.h"

int		cube(t_mlx *mlx, t_settings *settings)
{
	if (parser(settings) < 0)
	{
		parser_free(settings);
		return (-1);
	}
	if (canvas_init(mlx, settings) < 0)
		return (-1);
	start_game(mlx, settings->save);
	parser_free(settings);
	return (0);
}

int		cube_init(char *file, int save)
{
	t_mlx		mlx;
	t_settings	*settings;

	if (!(settings = (t_settings *)malloc(sizeof(t_settings))))
		return (MALLOC_ERROR);
	mlx_struct_init(&mlx);
	init_settings(settings);
	settings->file = file;
	settings->save = save;
	return (cube(&mlx, settings));
}

int		main(int ac, char **av)
{
	int save;

	save = 0;
	if ((ac < 2 || ac > 3) || !check_format(av[1], ".cub"))
	{
		print_error(ARG_ERROR);
		return (ARG_ERROR);
	}
	if (ac == 3)
	{
		if (ft_strncmp(av[2], "--save", ft_max(ft_strlen(av[2]), 6)) == 0)
			save = 1;
		else
		{
			print_error(ARG_ERROR);
			return (ARG_ERROR);
		}
	}
	cube_init(av[1], save);
	return (0);
}
