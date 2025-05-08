CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
#DEBUG_FLAGS = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=all
#DEBUG_FLAGS = valgrind --suppressions=./mini.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=all

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = Libft

SRCS = $(SRC_DIR)/builtins.c \
	$(SRC_DIR)/builtins_cd.c \
	$(SRC_DIR)/builtins_cd_path.c \
	$(SRC_DIR)/builtins_cd_utils.c \
	$(SRC_DIR)/builtins_echo.c \
	$(SRC_DIR)/builtins_env.c \
	$(SRC_DIR)/builtins_exit.c \
	$(SRC_DIR)/builtins_unset.c \
	$(SRC_DIR)/builtins_utils.c \
	$(SRC_DIR)/env_utils1.c \
	$(SRC_DIR)/env_utils2.c \
	$(SRC_DIR)/exec_external_cmd.c \
	$(SRC_DIR)/exec_extra1.c \
	$(SRC_DIR)/exec_fork.c \
	$(SRC_DIR)/exec_fork_pipeline.c \
	$(SRC_DIR)/exec_norm.c \
	$(SRC_DIR)/exec_pipes.c \
	$(SRC_DIR)/exec_rearrange_pipes.c \
	$(SRC_DIR)/exec_redirs.c \
	$(SRC_DIR)/expander.c \
	$(SRC_DIR)/expander_apply_split.c \
	$(SRC_DIR)/expander_expand.c \
	$(SRC_DIR)/expander_extra_extra.c \
	$(SRC_DIR)/expander_splitter.c \
	$(SRC_DIR)/expander_utils.c \
	$(SRC_DIR)/expander_utils2.c \
	$(SRC_DIR)/export.c \
	$(SRC_DIR)/export_extra_utils.c \
	$(SRC_DIR)/export_main.c \
	$(SRC_DIR)/export_utils.c \
	$(SRC_DIR)/frees.c \
	$(SRC_DIR)/frees_utils.c \
	$(SRC_DIR)/heredocs.c \
	$(SRC_DIR)/heredocs_input.c \
	$(SRC_DIR)/heredocs_more_utils.c \
	$(SRC_DIR)/heredocs_utils.c \
	$(SRC_DIR)/how_many_utils_do_i_need.c \
	$(SRC_DIR)/lex_utils.c \
	$(SRC_DIR)/lexer.c \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/main_utils.c \
	$(SRC_DIR)/memory.c \
	$(SRC_DIR)/parsing.c \
	$(SRC_DIR)/parsing_checks.c \
	$(SRC_DIR)/parsing_handles.c \
	$(SRC_DIR)/parsing_utils.c \
	$(SRC_DIR)/sighandle.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/builtins_utils2.c \
	$(SRC_DIR)/export_norm_utils.c \


OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT = $(LIBFT_DIR)/libft.a

NAME = minishell

define PIXEL_ART
@echo "\033[1;31mM\033[1;33mi\033[1;32mn\033[1;36mi\033[1;34ms\033[1;35mh\033[1;31me\033[1;33ml\033[1;32ml\033[0m"
@echo "\033[1;34m================\033[0m"
endef

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(PIXEL_ART) # Display colorful art
	@echo "Building $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)
	@echo "Done!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@make -s -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean

re: fclean all

#run: CFLAGS += $(DEBUG_FLAGS)
#run: all
#	@echo "Running $(NAME) with Valgrind..."
#	@$(DEBUG_FLAGS) ./$(NAME)

.PHONY: all clean fclean re
