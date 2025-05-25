# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/04 19:20:15 by hmensah-          #+#    #+#              #
#    Updated: 2025/05/25 21:48:11 by sngantch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft
INCLUDE_DIR = include/readline

# Determine OS for libraries
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	LIB_DIR = libs/linux
	LIBS = -L$(LIB_DIR) -lreadline -lhistory -lncurses
else
	LIB_DIR = libs/mac
	LIBS = -L$(LIB_DIR) -lreadline -lhistory -lcurses
endif

# Source files
#pipe_handler.c pipe_exec.c 
SRC_FILES =\
	main.c \
	builtins/builtins_utils.c builtins/env.c builtins/cd.c \
	builtins/echo.c builtins/exit.c builtins/export.c builtins/pwd.c \
	builtins/unset.c environments/env.c environments/env_util.c \
	signals/signal.c  \
	execution/expand_var.c execution/expand_var_util.c execution/sub_var.c \
	execution/pipe_utils.c execution/pipe_utils1.c execution/pipe_utils2.c \
	execution/pipe_exec.c \
	execution/exec_utils.c execution/run_io.c execution/logical_ops.c \
	execution/run_simple_cmd.c execution/run_command.c execution/exit_status.c execution/glob.c \
	utils/res_helpers.c utils/main_utils.c \
	parsing/lexer.c parsing/parser.c parsing/lex_extract.c parsing/lex_token.c \
	parsing/lex_token_util.c parsing/parser_cmd.c parsing/parser_util.c

# Object files# Object files
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -Isrc

# Libft library
LIBFT = $(LIBFT_DIR)/libft.a

# Rules
all: $(NAME)

$(NAME): $(OBJ_DIR) $(LIBFT) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT) $(LIBS) $(INCLUDES) -o $(NAME)
	@echo "Minishell compiled successfully!"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/parsing

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
