NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f
OBJ_DIR = ./obj

SRC = sources/main.c sources/parsing.c sources/error.c sources/split.c \
	  sources/utils.c sources/philosophers.c sources/dinner.c

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -pthread -o $(NAME)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re

