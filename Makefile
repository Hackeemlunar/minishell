NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft
INCLUDE_DIR = include

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
SRC_FILES = main.c \
			parsing/parser.c

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