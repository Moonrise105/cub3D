/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_03.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:19:03 by ctobias           #+#    #+#             */
/*   Updated: 2020/10/06 17:19:44 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void				get_ray_delta(t_ray *ray)
{
	if (ray->dir.y == 0)
		ray->delta.x = 0;
	else if (ray->dir.x == 0)
		ray->delta.x = 1;
	else
		ray->delta.x = fabs(1 / ray->dir.x);
	if (ray->dir.x == 0)
		ray->delta.y = 0;
	else if (ray->dir.y == 0)
		ray->delta.y = 1;
	else
		ray->delta.y = fabs(1 / ray->dir.y);
}
