BONUS_NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f
OBJ_DIR = ./obj

SRC = sources/main/main_bonus.c sources/main/parsing_bonus.c sources/main/philosophers_bonus.c \
	  sources/utils/split_bonus.c sources/utils/utils_bonus.c sources/utils/error_bonus.c \
	  sources/utils/time_bonus.c sources/forks/child_bonus.c sources/forks/supervise_bonus.c \
	  sources/forks/parent_bonus.c

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(BONUS_NAME)

bonus: $(BONUS_NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(BONUS_NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -pthread -o $(BONUS_NAME)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(BONUS_NAME)

re: fclean all

.PHONY: all bonus clean fclean re

