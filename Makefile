CC = cc

CFLAGS += -Wall -Werror -Wextra -fsanitize=address -g3

LDFLAGS += -lreadline -L/goinfre/$(USER)/homebrew/opt/readline/lib

INCLUDES += -I /goinfre/$(USER)/homebrew/opt/readline/include

NAME = minishell

LIBFT = libft.a
LIBFT_DIR = libft/

SRC =	main.c					\
		lexer/list.c			\
		lexer/lexer.c			\
		lexer/append_token.c	\
		lexer/is_redirection.c	\
		lexer/is_token.c		\
		lexer/syntax.c			\
		parser/parser.c			\
		parser/create_cmd.c		\
		parser/global.c			\
		parser/redir.c			\
		parser/is_type.c		\
		builtin/echo.c			\
		builtin/pwd.c			\
		builtin/exit.c			\
		builtin/cd.c			\
		builtin/export.c		\
		builtin/env.c			\
		builtin/unset.c			\
		builtin/export_sort.c	\
		exec/cmd.c				\
		utils/ft_isspace.c		\
		utils/get_env.c			\
		utils/debug_tools.c		\
		utils/check_plus.c		\
		utils/ft_strconcat.c	\
		heredoc/doc.c			\
		signal/signals.c		\
		lexer/lexer_len.c		\
        lexer/list_free.c		\
        lexer/wildcard.c		\

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) ./includes/minishell.h
	# @stty -echoctl
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(RFLAGS) $(INCLUDES) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)
	mv $(LIBFT_DIR)$(LIBFT) .

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re: clean fclean all minishell