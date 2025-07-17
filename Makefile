NAME = philo
CC = cc
CFLAGS = -Wall #-g3 -fsanitize=address#-Werror -Wextra -g3 
SRC = main.c ft_utils.c ft_utils1.c ft_utils2.c ft_utils3.c ft_routine.c ft_monitor.c ft_utils4.c
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


