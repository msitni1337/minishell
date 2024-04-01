LEXER_SRC = src/lexer/lexer.c src/lexer/main.c src/lexer/tokenizer.c src/lexer/utils.c src/shared/common.c
LEXER_OBJ = $(LEXER_SRC:.c=.o)

CC = cc
NAME = minishell
LEXER = lexer
CFLAGS = -Wall -Werror -Wextra -Iincludes -g3
LDFLAGS = -lreadline

.PHONY : re fclean clean all

%.o : %.c
	$(CC) $(CFLAGS) $< -c -o $@

all : $(LEXER)

# NOT IMPLEMENTED
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

$(LEXER): $(LEXER_OBJ)
	$(CC) $(CFLAGS) $(LEXER_OBJ) -o $(LEXER) $(LDFLAGS)

clean :
	rm -f $(LEXER_OBJ)

fclean : clean
	rm -f $(LEXER)

re : fclean all