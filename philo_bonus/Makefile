NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
OBJ_DIR = ./obj

SRC = sources/main/main_bonus.c sources/main/parsing_bonus.c sources/main/philosophers_bonus.c \
	  sources/utils/split_bonus.c sources/utils/utils_bonus.c sources/utils/error_bonus.c \
	  sources/utils/time_bonus.c sources/utils/free_bonus.c sources/forks/serve_food_bonus.c \
	  sources/forks/supervise_bonus.c sources/forks/philo_spawn_bonus.c \
	  sources/forks/death_meal_threads_bonus.c sources/forks/serve_food_utils_bonus.c

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

bonus: $(NAME)

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

