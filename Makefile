LEXER_SRC = src/lexer/lexer.c src/lexer/main.c src/lexer/tokenizer.c src/lexer/utils.c src/shared/common.c
LEXER_OBJ = $(LEXER_SRC:.c=.o)

LIBFT_DIR=lib/libft
LIBFT=libft.a

CC = cc
NAME = minishell
LEXER = lexer
CFLAGS = -Wall -Werror -Wextra -Iincludes -g3
LDFLAGS = -lreadline -L$(LIBFT_DIR) -lft


.PHONY : re fclean clean all

%.o : %.c
	$(CC) $(CFLAGS) $< -c -o $@

all : $(LEXER)

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