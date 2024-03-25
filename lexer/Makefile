SRC = main.c lexer.c
OBJ = $(SRC:.c=.o)
CC = cc
NAME = lexer
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline

.PHONY : re fclean clean all

%.o : %.c
	$(CC) $(CFLAGS) $< -c -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

all : $(NAME)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all