# Main Makefile
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
LINKFLAGS = -lreadline -g -L$(shell brew --prefix readline)/lib
# Source files
SRC_DIR = libft
SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS += main.c builtins.c counting.c echo.c env.c ft_execvp.c ft_piping.c \
handle_path.c handling_cmd1.c handling_cmd2.c heredoc.c history.c io_files.c \
minishell.c signals.c splitonsteroids.c termios.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(NAME)

# Compile the NAMEutable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LINKFLAGS) -o $@ $(OBJS)

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
