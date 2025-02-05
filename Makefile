NAME=pipex
CC=cc
CFLAGS= -Wall -Wextra -Werror
SRC= error_hand.c pipex.c\
	libft/ft_strncmp.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strlen.c libft/ft_strdup.c libft/ft_strlcpy.c
OBJ=$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c libft/libft.h pipex.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean:clean
	rm -rf $(NAME)

re:fclean all