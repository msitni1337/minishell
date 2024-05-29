# LIBS
LIBFT_DIR=lib/libft
LIBFT=libft.a

# SRCS
SHARED_SRC = $(wildcard src/shared/*.c)
SHARED_OBJ = $(SHARED_SRC:.c=.o)

SHELL_SRC = $(wildcard src/shell/*.c)
SHELL_OBJ = $(SHELL_SRC:.c=.o)

ENV_SRC = $(wildcard src/environments/*.c)
ENV_OBJ = $(ENV_SRC:.c=.o)

BUILTIN_SRC = $(wildcard src/built-ins/*.c)
BUILTIN_OBJ = $(BUILTIN_SRC:.c=.o)

LEXER_SRC = $(wildcard src/lexer/*.c)
LEXER_OBJ = $(LEXER_SRC:.c=.o)

EXEC_SRC = $(wildcard src/interpreter/*.c)
EXEC_OBJ = $(EXEC_SRC:.c=.o)

TEST_SRC = tests/lexer/tree_printing.c
TEST_OBJ = $(TEST_SRC:.c=.o)

OBJ = $(SHARED_OBJ) $(SHELL_OBJ) $(BUILTIN_OBJ) $(ENV_OBJ) $(LEXER_OBJ) $(EXEC_OBJ) $(TEST_OBJ)

# VARS
CC = cc
NAME = minishell
LEXER = lexer
BUILTIN = builtin
CFLAGS = -Wall -Werror -Wextra -Iincludes -g3 -fsanitize=address
LDFLAGS = -lreadline -L$(LIBFT_DIR) -lft


.PHONY : re fclean clean all

%.o : %.c
	$(CC) $(CFLAGS) $< -c -o $@

all : $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

$(BUILTIN): $(LIBFT) $(LEXER_OBJ) $(ENV_OBJ) $(BUILTIN_OBJ)
	$(CC) $(CFLAGS) $(LEXER_OBJ) $(ENV_OBJ) $(BUILTIN_OBJ) -o $(BUILTIN) $(LDFLAGS)

$(LEXER): $(LIBFT) $(SHARED_OBJ) $(tests) $(LEXER_OBJ)
	$(CC) $(CFLAGS) $(LEXER_OBJ) -o $(LEXER) $(LDFLAGS)

$(LIBFT):
	make -C ${LIBFT_DIR}

clean :
	make clean -C ${LIBFT_DIR}
	rm -f $(OBJ)

fclean : clean
	make fclean -C ${LIBFT_DIR}
	rm -f $(NAME)
	rm -f $(EXEC)
	rm -f $(BUILTIN)
	rm -f $(LEXER)

re : fclean all