int main_alg(t_mlx *mlx)
{
	int x = 0;
	double cameraX;
	double rayDirX;
	double rayDirY;
	int hit;
	double deltaDistX;
	double deltaDistY;
	int mapX;
	int mapY;
	double sideX;
	double sideY;
	double perpWallDist;
	int side;
	int stepX;
	int stepY;
	int lineHeight;
	int drawStart;
	int drawEnd;
	double wallX;
	int texX;
	int texY;
	double step;
	double texPos;

	int texWidth = 32;
	int texHeight = 32;

	int tx;
	int ty;

	double zBuffer[WIN_WIDTH];

	clear_data(mlx);
	//floor
	//int y = WIN_HEIGHT / 2;
	int y = 0;
	while (y < WIN_HEIGHT)
	{
		float rayDirX0 = mlx->player.dir.x - mlx->player.plane.x;
		float rayDirX1 = mlx->player.dir.x + mlx->player.plane.x;
		float rayDirY0 = mlx->player.dir.y - mlx->player.plane.y;
		float rayDirY1 = mlx->player.dir.y + mlx->player.plane.y;
		int p = y - WIN_HEIGHT / 2;
		float posZ = 0.5 * WIN_HEIGHT;
		float rowDist = posZ / p;
		float floorStepX = rowDist * (rayDirX1 - rayDirX0) / WIN_WIDTH;
		float floorStepY = rowDist * (rayDirY1 - rayDirY0) / WIN_WIDTH;
		float floorX = mlx->player.pos.x + rowDist * rayDirX0;
		float floorY = mlx->player.pos.y + rowDist * rayDirY0;
		x = 0;
		while (x < WIN_WIDTH)
		{
			int cellX = (int)floorX;
			int cellY = (int)floorY;
			tx = (int)(32 * (floorX - cellX)) & 31;
			ty = (int)(32 * (floorY - cellY)) & 31;
			if (ty < 0)
				ty = 31;
			floorX += floorStepX;
			floorY += floorStepY;
			int color = mlx->texture_NO.data[ty * 32 + tx];
			color = (color >> 1) & 8355711;
			mlx->img.data[y * WIN_WIDTH + x] = color;
			++x;
		}
		//printf("tx = %d\n", tx);
		++y;
	}
	//floorend

	//walls
	x = 0;
	y = 0;
	while (x < WIN_WIDTH)
	{
		cameraX = 2 * x / (double)WIN_WIDTH - 1;
		rayDirX = mlx->player.dir.x + mlx->player.plane.x * cameraX;
		rayDirY = mlx->player.dir.y + mlx->player.plane.y * cameraX;
		mapX = (int)mlx->player.pos.x;
		mapY = (int)mlx->player.pos.y;
		deltaDistX = (rayDirY == 0) ? 0 : ((rayDirX == 0) ? 1 : fabs(1 / rayDirX));
		deltaDistY = (rayDirX == 0) ? 0 : ((rayDirY == 0) ? 1 : fabs(1 / rayDirY));
		hit = 0;
		if (rayDirX < 0)
		{
			stepX = -1;
			sideX = (mlx->player.pos.x - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideX = (mapX + 1.0 - mlx->player.pos.x) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideY = (mlx->player.pos.y - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideY = (mapY + 1.0 - mlx->player.pos.y) * deltaDistY;
		}

		while (!hit)
		{
			if (sideX < sideY)
			{
				sideX += deltaDistX;
				mapX += stepX;
				if (rayDirX > 0)
					side = 0;
				else
					side = 1;
			}
			else
			{
				sideY += deltaDistY;
				mapY += stepY;
				if (rayDirY > 0)
					side = 2;
				else
					side = 3;
			}
			if (worldMap[mapX][mapY] > 0)
				hit = 1;
		}
		t_img texture;

		if (side == 1)
			texture = mlx->texture_SO;
		else if (side == 0)
			texture = mlx->texture_WE;
		else if (side == 2)
			texture = mlx->texture_EA;
		else if (side == 3)
			texture = mlx->texture_NO;

		if (side == 0 || side == 1)
			perpWallDist = (mapX - mlx->player.pos.x + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - mlx->player.pos.y + (1 - stepY) / 2) / rayDirY;
		lineHeight = (int)(WIN_HEIGHT / perpWallDist);
		drawStart = -lineHeight / 2 + WIN_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + WIN_HEIGHT / 2;
		if (drawEnd >= WIN_HEIGHT)
			drawEnd = WIN_HEIGHT - 1;

		if (side == 0 || side == 1)
			wallX = mlx->player.pos.y + perpWallDist * rayDirY;
		else
			wallX = mlx->player.pos.x + perpWallDist * rayDirX;
		wallX -= floor(wallX);
		texX = (int)(wallX * (double)texWidth);
		if (side == 0 || side == 3)
			texX = texWidth - texX - 1;
		step = 1.0 * texHeight / lineHeight;
		texPos = (drawStart - WIN_HEIGHT / 2 + lineHeight / 2) * step;
		int y = drawStart;
		int color;
		while (y < drawEnd)
		{
			//texY = (int)texPos & (texHeight - 1);
			texY = (int)texPos;
			texPos += step;
			color = texture.data[(texY)*texHeight + (texX)];
			mlx->img.data[y * WIN_WIDTH + x] = color;
			++y;
		}
		zBuffer[x] = perpWallDist; //for sprites
		++x;
	}
	//walls end

	//texture_sprite declaration
	t_sprite *sprites;
	int num_sprites = 3;
	sprites = malloc(sizeof(t_sprite) * num_sprites);
	sprites[0].x = 3;
	sprites[0].y = 3;

	sprites[1].x = 20;
	sprites[1].y = 20;
	sprites[2].x = 19;
	sprites[2].y = 19;
	//texture_sprite declaration end

	//texture_sprite
	int n;
	double spriteX;
	double spriteY;
	double invDet;
	double transformX;
	double transformY;
	int spriteScreenX;
	int spriteHeight;
	int drawStartY;
	int drawEndY;
	int spriteWidth;
	int drawStartX;
	int drawEndX;
	int stripe;
	int spriteTexX;

	n = 0;
	while (n < num_sprites)
	{
		sprites[n].dist = (mlx->player.pos.x - sprites[n].x) * (mlx->player.pos.x - sprites[n].x) + (mlx->player.pos.y - sprites[n].y) * (mlx->player.pos.y - sprites[n].y);
		++n;
	}
	sort_sprites(sprites, 0, num_sprites - 1);
	n = num_sprites;
	while (n >= 0)
	{
		spriteX = sprites[n].x - mlx->player.pos.x;
		spriteY = sprites[n].y - mlx->player.pos.y;
		invDet = 1.0 / (mlx->player.plane.x * mlx->player.dir.y - mlx->player.plane.y * mlx->player.dir.x);
		transformX = invDet * (mlx->player.dir.y * spriteX - mlx->player.dir.x * spriteY);
		transformY = invDet * ((-1) * mlx->player.plane.y * spriteX + mlx->player.plane.x * spriteY);
		spriteScreenX = (int)((WIN_WIDTH / 2) * (1 + transformX / transformY));
		spriteHeight = abs((int)(WIN_HEIGHT / transformY));
		drawStartY = -spriteHeight / 2 + WIN_HEIGHT / 2;
		if (drawStartY < 0)
			drawStartY = 0;
		drawEndY = spriteHeight / 2 + WIN_HEIGHT / 2;
		if (drawEndY >= WIN_HEIGHT)
			drawEndY = WIN_HEIGHT - 1;
		spriteWidth = abs((int)(WIN_HEIGHT / transformY));
		drawStartX = -spriteWidth / 2 + spriteScreenX;
		if (drawStartX < 0)
			drawStartX = 0;
		drawEndX = spriteWidth / 2 + spriteScreenX;
		if (drawEndX >= WIN_WIDTH)
			drawEndX = WIN_WIDTH - 1;
		stripe = drawStartX;
		while (stripe < drawEndX)
		{
			spriteTexX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * 64 / spriteWidth) / 256;
			if (transformY > 0 && stripe > 0 && stripe < WIN_WIDTH && transformY < zBuffer[stripe])
			{
				int q = drawStartY;
				while (q < drawEndY)
				{
					int d = q * 256 - WIN_HEIGHT * 128 + spriteHeight * 128;
					int spriteTexY = ((d * 64) / spriteHeight) / 256;
					int color = mlx->texture_sprite.data[64 * spriteTexY + spriteTexX];
					if (color != 0)
						mlx->img.data[q * WIN_WIDTH + stripe] = color;
					++q;
				}
			}
			++stripe;
		}
		//printf("%d  %d  %d  %d", drawStartX, drawEndX, drawStartY, drawEndY);
		--n;
	}
	//texture_sprite end
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win, mlx->img.img_ptr, 0, 0);
	//printf("%f  %f  %f  %f\n", mlx->player.pos.x, mlx->player.pos.y, mlx->player.dir.x, mlx->player.dir.y);
	return (0);
}