SRCS			= main.c get_next_line.c parser.c bmp.c parse_map.c parse_line.c \
				utils_00.c utils_01.c sort.c walls.c sprites.c floor.c \
				set.c
				
OBJS			= $(SRCS:.c=.o)
LIBFT			= libft
CC				= gcc
RM				= rm -f
# CFLAGS			= -O3 -Wall -Wextra -Werror -I.
CFLAGS			= -g -O3 -I.
LIBS			= -Lmlx -lmlx -framework OpenGL -framework AppKit -lm
MLX				= libmlx.dylib

NAME			= cub3D

all:			$(NAME)

$(NAME):		$(MLX) $(OBJS)
				make bonus -C $(LIBFT)
				cp libft/libft.a ./libft.a
				gcc ${CFLAGS} -o ${NAME} libft.a ${OBJS} ${LIBS}

$(MLX):
				@$(MAKE) -C mlx
				@mv mlx/$(MLX) .

clean:
				@$(MAKE) -C mlx clean
				$(RM) $(OBJS) $(BONUS_OBJS)
				$(MAKE) -C $(LIBFT) clean

fclean:			clean
				$(RM) $(NAME) $(MLX)
				$(MAKE) -C $(LIBFT) fclean

re:				fclean $(NAME)

.PHONY:			all clean fclean re