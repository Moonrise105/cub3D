#include "cub.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
typedef struct s_bfh
{
	
} t_bfh;

typedef struct s_bih
{

} t_bih;

void	bdata(int fd, t_mlx *mlx)
{
	int				i;
	int				j;
	unsigned char	buffer[4];

	i = mlx->img.width * (mlx->img.height - 1);
	while (i >= 0)
	{
		j = 0;
		while (j < mlx->img.width)
		{
			buffer[0] = (unsigned char)(mlx->img.data[i] % 256);
			buffer[1] = (unsigned char)(mlx->img.data[i] / 256 % 256);
			buffer[2] = (unsigned char)(mlx->img.data[i] / 256 / 256 % 256);
			buffer[3] = (unsigned char)(0);
			write(fd, buffer, 4);
			i++;
			j++;
		}
		i -= 2 * mlx->img.width;
	}
}
void	bih(int fd, t_mlx *mlx)
{
	int				n;
	unsigned char	header[40];

	n = 0;
	while (n < 40)
		header[n++] = (unsigned char)(0);
	header[0] = (unsigned char)(40);
	n = mlx->img.width;
	header[4] = (unsigned char)(n % 256);
	header[5] = (unsigned char)(n / 256 % 256);
	header[6] = (unsigned char)(n / 256 / 256 % 256);
	header[7] = (unsigned char)(n / 256 / 256 / 256);
	n = mlx->img.height;
	header[8] = (unsigned char)(n % 256);
	header[9] = (unsigned char)(n / 256 % 256);
	header[10] = (unsigned char)(n / 256 / 256 % 256);
	header[11] = (unsigned char)(n / 256 / 256 / 256);
	header[12] = (unsigned char)(1);
	header[14] = (unsigned char)(32);
	write(fd, header, 40);
}

void	bfh(int fd, t_mlx *mlx)
{
	int n;
	unsigned char header[14];

	n = 0;
	while (n < 14)
	{
		header[n] = (unsigned char)0;
		++n;
	}
	header[0] = (unsigned char)66;
	header[1] = (unsigned char)77;
	n = mlx->map->width * mlx->map->height * 4 + 54 ;
	header[2] = (unsigned char)(n % 256);
	header[3] = (unsigned char)(n / 256 % 256);
	header[4] = (unsigned char)(n / 256 / 256 % 256);
	header[5] = (unsigned char)(n / 256 / 256 / 256);
	header[10] = (unsigned char)(54);
	write(fd, header, 14);
	header[2] = n % 2;

}

int		bitmap(t_mlx *mlx)
{
	int fd;

	fd = open("bit.bmp", O_CREAT | O_WRONLY | O_TRUNC);
	bfh(fd, mlx);
	bih(fd, mlx);
	bdata(fd, mlx);
	close(fd);
	
	return (0);
}
