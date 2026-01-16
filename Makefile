# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/15 14:54:57 by jotong            #+#    #+#              #
#    Updated: 2026/01/16 13:16:54 by jotong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INC				= -Iinclude
SRC_DIR			= src
PARSER_DIR		= parser
HISTORY_DIR		= history
EXECUTOR_DIR	= executor
LEXER_DIR		= lexer
BUILTINS_DIR	= builtins

# Library folders
LIBFT_DIR		= lib/libft
LIBFT			= ${LIBFT_DIR}/libft.a

OBJ_DIR			= obj
FLAG			= -Wall -Wextra -Werror -I$(LIBFT_DIR) $(INC) -g
NAME			= minishell

PARSER_FILES	= $(SRC_DIR)/$(PARSER_DIR)/parser_new.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_utils_new.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_cmd_redirect_new.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_cmd_redirect_utils_new.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_cmd_redirect_utils_1.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_cmd_redirect_1.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_heredoc.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_one_redirect.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_helper.c \
					$(SRC_DIR)/$(PARSER_DIR)/parser_node_utils_new.c
LEXER_FILES		= $(SRC_DIR)/$(LEXER_DIR)/lexer.c \
					$(SRC_DIR)/$(LEXER_DIR)/lexer_checks.c \
					$(SRC_DIR)/$(LEXER_DIR)/lexer_checks2.c \
					$(SRC_DIR)/$(LEXER_DIR)/lexer_checks3.c \
					$(SRC_DIR)/$(LEXER_DIR)/lexer_utils.c \
					$(SRC_DIR)/$(LEXER_DIR)/lexer_utils2.c \
					$(SRC_DIR)/$(LEXER_DIR)/lexer_utils3.c \
					$(SRC_DIR)/$(LEXER_DIR)/lexer_for_heredoc.c \
					$(SRC_DIR)/$(LEXER_DIR)/tokenizer.c
BUILTINS_FILES	= $(SRC_DIR)/$(BUILTINS_DIR)/cd.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/echo.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/env.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/env2.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/env3.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/env4.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/builtin-utils.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/builtin-utils2.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/builtin-utils3.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/builtin-utils4.c \
					$(SRC_DIR)/$(BUILTINS_DIR)/builtin-utils5.c
HISTORY_FILES	= $(SRC_DIR)/$(HISTORY_DIR)/history.c
EXECUTOR_FILES	= $(SRC_DIR)/$(EXECUTOR_DIR)/executor.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/executor_utils.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/executor_utils2.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/cmd_exec.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/cmd_exec_helper.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/pipe_exec.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/redir_exec.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/logical_exec.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand2.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand3.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand4.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand5.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand6.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand7.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand8.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand9.c \
					$(SRC_DIR)/$(EXECUTOR_DIR)/strip_expand10.c
REQ_SRC			:= $(SRC_DIR)/main.c $(SRC_DIR)/signals.c \
					$(SRC_DIR)/shell_utils.c $(SRC_DIR)/shell_utils_2.c \
					$(SRC_DIR)/cleanup_utils.c \
					$(SRC_DIR)/print_utils.c \
					$(SRC_DIR)/print_utils1.c \
					$(SRC_DIR)/print_utils2.c \
					$(SRC_DIR)/print_utils3.c \
					$(SRC_DIR)/print_utils4.c \
					$(PARSER_FILES) $(LEXER_FILES) \
					$(HISTORY_FILES) $(EXECUTOR_FILES) \
					$(BUILTINS_FILES)
HEADER			= minishell.h
OBJ 			= $(REQ_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# OBJ				= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(REQ_SRC))

$(info REQ_SRC = $(REQ_SRC))
$(info OBJ = $(OBJ))

# BONUS folders
BONUS_FOLDER	= bonus

# BONUS Server files
# FLAG_BONUS		= -Wall -Wextra -Werror -I../$(LIBFT_FOLDER)
# REQ_SRC_S_BONUS	=
# HEADER_BONUS	= $(BONUS_FOLDER)/minishell.h
# OBJ_BONUS		= $(addprefix $(BONUS_FOLDER)/,$(REQ_SRC_S_BONUS:.c=.o))

# Default target
all: $(LIBFT) $(NAME)

# Compile and archive .o files
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(FLAG) $(LIBFT) -lreadline -o $(NAME)

# Libft
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Compile .c to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(FLAG) -c $< -o $@

# Compile .c to .o
# $(BONUS_FOLDER)/%.o : $(BONUS_FOLDER)/%.c
# 	@$(CC) $(FLAG) -c $< -o $@

# BONUS: Compile and archive .o files
# $(NAME_BONUS): $(OBJ_BONUS) $(LIBFT)
# 	$(CC) $(OBJ_BONUS) $(FLAG_BONUS) $(LIBFT) -o $(NAME_BONUS)

# Bonus rules
# bonus:
# 	@$(MAKE) $(NAME_BONUS)

# Clean OBJ files
clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
# @rm -f $(OBJ) $(OBJ_BONUS)

# Clean all
fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

# Used to declare targets that are not actual files but command names
.PHONY: all clean fclean re
