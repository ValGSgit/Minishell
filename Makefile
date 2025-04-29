# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -g
DEBUG_FLAGS = -g -fsanitize=address

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = Libft

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Library
LIBFT = $(LIBFT_DIR)/libft.a

# Executable name
NAME = minishell

# Colorful "Minishell" pixel art
define PIXEL_ART
@echo "\033[1;31mM\033[1;33mi\033[1;32mn\033[1;36mi\033[1;34ms\033[1;35mh\033[1;31me\033[1;33ml\033[1;32ml\033[0m"
@echo "\033[1;34m================\033[0m"
endef

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(PIXEL_ART) # Display colorful art
	@echo "Building $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)
	@echo "Done!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ # Silent compilation

$(LIBFT):
	@make -s -C $(LIBFT_DIR) # Silent make in ShellLibft

clean:
	@rm -rf $(OBJ_DIR)
	@make -s -C $(LIBFT_DIR) clean # Silent clean in ShellLibft

fclean: clean
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean # Silent fclean in ShellLibft

re: fclean all

debug: CFLAGS += $(DEBUG_FLAGS)
debug: re

.PHONY: all clean fclean re debug
