# Main Makefile

NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

# Source files
SRC_DIR = libft
SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS += minishell.c termios.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(NAME)

# Compile the NAMEutable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile individual source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the NAMEutable
fclean:
	rm -f $(OBJS) $(NAME)
#	rm -f libft/*.o libft/libft.a
clean:
	rm -f $(OBJS)

re: fclean all

# Phony targets
.PHONY: all fclean clean

# Include sub-Makefiles
#include libft/Makefile