# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ctobias <ctobias@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/22 18:29:59 by ctobias           #+#    #+#              #
#    Updated: 2020/10/22 19:38:17 by ctobias          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			= main.c get_next_line.c parser.c bmp.c parse_map.c parse_line.c \
				utils_00.c utils_01.c sort.c walls.c sprites.c floor.c \
				set.c utils_02.c move.c screen.c utils_03.c
				
OBJS			= $(SRCS:.c=.o)
LIBFT			= libft
CC				= gcc
RM				= rm -f
CFLAGS			= -O3 -Wall -Wextra -Werror -I.
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
				$(RM) libft.a

re:				fclean $(NAME)

.PHONY:			all clean fclean re