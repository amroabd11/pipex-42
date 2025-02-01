NAME=pipex
CC=cc
CFLAGS= -Wall -Wextra -Werror
SRC= error_hand.c pipex.c
DIR=./libft
LIBFT=$(DIR)/libft.a
OBJ=$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) libft/libft.h pipex.h
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(DIR)

clean:
	rm -r $(OBJ)
	make -C clean $(DIR)

fclean:clean
	rm -rf $(NAME)
	make -C fclean $(DIR)

re:fclean all