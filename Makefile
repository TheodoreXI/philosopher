NAME = philo
CC = cc
CFLAGS = -Wall -Werror -Wextra #-g3 -fsanitize=address #-fsanitize=thread -fsanatize=
SRC = main.c ft_utils.c ft_utils1.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c philosopher.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all


