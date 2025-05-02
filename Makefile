# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmensah- <hmensah-@student.42abudhabi.ae>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/02 20:55:00 by hmensah-          #+#    #+#              #
#    Updated: 2025/05/02 21:33:43 by hmensah-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

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
SRC_FILES =	main.c \
		utils/res_helpers.c \
		parsing/lexer.c parsing/parser.c

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
