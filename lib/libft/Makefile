# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/11 23:37:08 by jotong            #+#    #+#              #
#    Updated: 2025/11/14 19:55:46 by ksng             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAG 		= -Wall -Wextra -Werror

NAME 		= libft.a

REQ_SRC 	= ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c \
			ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c \
			ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c \
			ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c \
			ft_strchr.c ft_striteri.c ft_strlcat.c ft_strlcpy.c ft_strlen.c \
			ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strjoin.c \
			ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_calloc.c \
			ft_strdup.c ft_printf.c ft_p_nbr_base.c ft_p_nbr.c ft_p_str.c \
			ft_p_ptr.c ft_p_unbr.c get_next_line.c ft_strcmp.c
OBJ 		= $(REQ_SRC:.c=.o)

# Bonus files
BONUS_SRC	= ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
			ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstlast_bonus.c \
			ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
			ft_lstnew_bonus.c ft_lstsize_bonus.c
BONUS_OBJ	= $(BONUS_SRC:.c=.o)

HEADER		= libft.h

# Default target
all: $(NAME)

# Compile and archive .o files into libft.a
$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)

# Compile .c to .o
%.o: %.c ${HEADER}
	@cc $(FLAG) -c $< -o $@

# Bonus target - compile + add bonus .o files to libft.a
bonus: $(OBJ) $(BONUS_OBJ)
	@ar rc $(NAME) $(OBJ) $(BONUS_OBJ)
	@ranlib $(NAME)

# Clean OBJ files
clean:
	@rm -f $(OBJ) $(BONUS_OBJ)

# Clean all
fclean: clean
	@rm -f $(NAME) $(BONUS_OBJ)

# Rebuild everything
re: fclean all

# Used to declare targets that are not actual fieles but command names
.PHONY: all bonus clean fclean re
