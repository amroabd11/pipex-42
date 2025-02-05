NAME=pipex
CC=cc
CFLAGS= -Wall -Wextra -Werror
SRC= error_hand.c pipex.c\
	ft_strncmp.c ft_split.c ft_strjoin.c ft_strlen.c ft_strdup.c ft_strlcpy.c ft_putstr_fd.c
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