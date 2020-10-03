/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/31 18:09:38 by ctobias           #+#    #+#             */
/*   Updated: 2020/09/23 18:27:53 by ctobias          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./get_next_line.h"

char		*ft_strchr(const char *s, int c)
{
	char *p;

	p = (char*)s;
	while (*p && (*p != (char)c))
		p++;
	if (*p == (char)c)
		return (p);
	return (NULL);
}

size_t		ft_strlen(const char *str)
{
	size_t k;

	k = 0;
	if (str)
	{
		while (*str)
		{
			k++;
			str++;
		}
	}
	return (k);
}

char		*ft_strjoin(char const *s1, char const *s2)
{
	char *str;
	char *p;

	if (!s1 && !s2)
		return (NULL);
	str = (char*)malloc(sizeof(char) * \
	((s1 ? ft_strlen(s1) : 0) + (s2 ? ft_strlen(s2) : 0) + 1));
	if (!str)
		return (NULL);
	p = str;
	if (s1)
	{
		while (*s1)
			*str++ = *s1++;
	}
	if (s2)
	{
		while (*s2)
			*str++ = *s2++;
	}
	*str = '\0';
	return (p);
}

char		*ft_strdup(const char *str)
{
	int		len;
	char	*new_str;
	char	*p;

	len = ft_strlen(str);
	new_str = (char*)malloc(sizeof(char) * len + 1);
	p = new_str;
	if (!new_str)
		return (NULL);
	while (*str)
	{
		*new_str = *str;
		new_str++;
		str++;
	}
	*new_str = *str;
	return (p);
}

size_t		ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < (size - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = 0;
	}
	while (src[i])
		i++;
	return (i);
}
