# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/06 13:05:58 by vagarcia          #+#    #+#              #
#    Updated: 2025/03/26 09:54:02 by vagarcia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror -Isrc -Iinclude

VPATH			=	src:src/conversion:src/list:src/memory:src/output:src/string

CFILES = ft_isalpha.c ft_isascii.c ft_isalnum.c ft_isdigit.c \
	 ft_isprint.c ft_tolower.c ft_toupper.c ft_strlcat.c \
	 ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strdup.c \
	 ft_strchr.c ft_memset.c ft_memcpy.c ft_bzero.c \
	 ft_atoi.c ft_calloc.c ft_strrchr.c ft_strlcpy.c \
	 ft_memchr.c ft_memcmp.c ft_memmove.c ft_putchar_fd.c \
	 ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_substr.c \
	 ft_striteri.c ft_strmapi.c ft_strnstr.c ft_strjoin.c \
	 ft_strtrim.c ft_itoa.c ft_split.c get_next_line_bonus.c \
	 get_next_line_utils_bonus.c ft_strcmp.c ft_strndup.c \
	 ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
	 ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
	 ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c ft_strcpy.c \


ALLCFILES = $(CFILES:.c=.o)

OBJS			=	$(addprefix obj/,$(SRCS:.c=.o))

.PHONY: all clean fclean re

all:	$(NAME) 

$(NAME):	$(ALLCFILES)
			$(AR) -rcs $@ $?
			@echo "$(GREEN) MegaLibft compiled! $(DEF_COLOR)"

obj:
				mkdir -p obj

obj/%.o:		%.c | obj
				mkdir -p $(dir $@)
				$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(ALLCFILES)
	@echo "$(MAGENTA) Erasing object files for MegaLibft... $(DEF_COLOR)"

fclean : clean
	rm -f $(NAME)
	@echo "$(MAGENTA) Everything ready for re-make $(DEF_COLOR)"

re: fclean all
	@echo "$(CYAN) Cleaned and redid everything for MegaLibft..."
