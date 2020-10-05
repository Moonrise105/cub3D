#include "cub.h"

static void swap(t_sprite *a, t_sprite *b)
{
	t_sprite tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void merge_swap(t_sprite *sprites, t_sprite *tmp, int l, int r)
{
	int i;
	int j;
	int k;
	int mid;

	mid = l + (r - l) / 2;
	i = l;
	j = 0;
	k = mid - l + 1;
	while (i <= r)
	{
		if (j > mid - l)
			sprites[i] = tmp[k++];
		else if (k > r - l)
			sprites[i] = tmp[j++];
		else
			sprites[i] = (tmp[j].dist < tmp[k].dist) ? tmp[j++] : tmp[k++];
		++i;
	}
}

static int merge(t_sprite *sprites, int l, int r, int mid)
{
	int i;
	int j;
	int k;
	t_sprite *tmp;

	if (l >= r || mid < l || mid > r)
		return (0);
	if ((r == l + 1) && (sprites[l].dist > sprites[r].dist))
	{
		swap(&sprites[l], &sprites[r]);
		return (0);
	}
	tmp = malloc(sizeof(t_sprite) * (r - l + 1));
	if (!tmp)
		return (MALLOC_ERROR);
	i = l;
	while (i < r + 1)
	{
		tmp[i - l] = sprites[i];
		++i;
	}
	merge_swap(sprites, tmp, l, r);
	ft_free(tmp);
	return (0);
}

int  sort_sprites(t_sprite *sprites, int l, int r)
{
	int mid;

	if (l >= r)
		return (0);
	mid = l + (r - l) / 2;
	sort_sprites(sprites, l, mid);
	sort_sprites(sprites, mid + 1, r);
	return (merge(sprites, l, r, mid));
}