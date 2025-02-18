NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
OBJ_DIR = ./obj

SRC = sources/main/main.c sources/main/parsing.c sources/main/philosophers.c \
	  sources/utils/split.c sources/utils/utils.c sources/utils/error.c \
	  sources/utils/time.c  sources/forks/forks.c \

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

