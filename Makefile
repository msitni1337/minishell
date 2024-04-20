LEXER_SRC = src/lexer/lexer.c src/lexer/tokenizer.c src/lexer/tree_builder.c src/lexer/utils.c src/shared/common.c src/shared/dynamic_arrays.c src/shared/free.c src/shared/garbage_collector.c src/shell/initialization.c src/shell/signals.c

LEXER_OBJ = $(LEXER_SRC:.c=.o)

LIBFT_DIR=lib/libft
LIBFT=libft.a

EXEC_SRC = $(wildcard src/environments/*.c)
EXEC_OBJ = $(EXEC_SRC:.c=.o)

CC = cc
NAME = minishell
LEXER = lexer
EXEC = exec
CFLAGS = -Wall -Werror -Wextra -Iincludes -g3 -fsanitize=address
LDFLAGS = -lreadline -L$(LIBFT_DIR) -lft


.PHONY : re fclean clean all

%.o : %.c
	$(CC) $(CFLAGS) $< -c -o $@

all : $(EXEC)

$(EXEC): $(LEXER_OBJ) $(EXEC_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(LEXER_OBJ) $(EXEC_OBJ) -o $(EXEC) $(LDFLAGS)

# NOT IMPLEMENTED

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

$(LEXER): $(LEXER_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(LEXER_OBJ) -o $(LEXER) $(LDFLAGS)

$(LIBFT):
	make -C ${LIBFT_DIR}

clean :
	make clean -C ${LIBFT_DIR}
	rm -f $(LEXER_OBJ)

fclean : clean
	make fclean -C ${LIBFT_DIR}
	rm -f $(LEXER)

re : fclean all