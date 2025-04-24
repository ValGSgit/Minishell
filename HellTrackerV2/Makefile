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
SRC_FILES = builtins.c builtins_1.c builtins_2.c builtins_utils.c cmd_path.c debug.c \
           env_modify.c env_update.c env_utils1.c env_utils2.c executor.c \
           exec_child.c exec_fork.c exec_fork_utils.c exec_norm.c exec_pipes.c exec_redirs.c \
           expander.c expander_apply_split.c expander_expand.c expander_expand_1.c \
           expander_expand_2.c expander_splitter.c export.c export_utils.c frees.c \
           heredocs.c heredocs_extra_utils.c heredocs_more_utils.c heredocs_process.c \
           heredocs_utils.c lexer.c lex_utils.c main.c malloc_tracker.c memory_manager.c \
           parsing.c parsing_checks.c parsing_handles.c parsing_utils.c pipeline_utils.c \
           sighandle.c utils.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
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
