CC = cc

CFLAGS += -Wall -Werror -Wextra -fsanitize=address -g

LDFLAGS += -lreadline -L/goinfre/abeihaqi/homebrew/opt/readline/lib

INCLUDES += -I /goinfre/abeihaqi/homebrew/opt/readline/include

NAME = minishell

LIBFT = libft.a
LIBFT_DIR = libft/

SRC =	main.c					\
		lexer/list.c			\
		lexer/lexer.c			\
		lexer/append_token.c	\
		lexer/is_redirection.c	\
		lexer/is_token.c		\
		parser/parser.c			\
		builtin/chdir.c			\
		builtin/echo.c			\
		builtin/pwd.c			\
		utils/ft_isspace.c		\
		utils/get_env.c			\
		

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	# @stty -echoctl
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(RFLAGS) $(INCLUDES) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
	mv $(LIBFT_DIR)$(LIBFT) .

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re: clean fclean all minishell