# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/04 11:54:50 by rouarrak          #+#    #+#              #
#    Updated: 2023/07/13 02:14:39 by abeihaqi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS += -Wall -Werror -Wextra #-fsanitize=address -g3

LDFLAGS += -lreadline -L/goinfre/$(USER)/homebrew/opt/readline/lib

INCLUDES += -I /goinfre/$(USER)/homebrew/opt/readline/include

NAME = minishell

BONUS = minishell_bonus

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
		exec/cmd_util.c			\
		exec/executers.c		\
		exec/cmd.c				\
		exec/executing.c		\
		utils/ft_isspace.c		\
		utils/get_env.c			\
		utils/debug_tools.c		\
		utils/debug_tools2.c	\
		utils/check_plus.c		\
		utils/ft_strconcat.c	\
		utils/sets.c			\
		utils/piping.c			\
		heredoc/here.c			\
		heredoc/doc.c			\
		heredoc/hd_utils.c		\
		signal/signals.c		\
		lexer/lexer_len.c		\
        lexer/list_free.c		\
        lexer/wildcard.c		\
		lexer/syntax_utils.c	\
		lexer/lexer_env.c		\
		lexer/free_ast.c		\
		lexer/wildcard_helper.c	\
		lexer/wildcard_helper2.c\

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

all: $(NAME) $(BONUS)

$(NAME): $(OBJ) $(LIBFT) ./includes/minishell.h
	# @stty -echoctl
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(RFLAGS) $(INCLUDES) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)
	mv $(LIBFT_DIR)$(LIBFT) .

bonus: $(BONUS)

$(BONUS): $(OBJ) $(LIBFT) ./includes/minishell.h
	# @stty -echoctl
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(RFLAGS) $(INCLUDES) $(LDFLAGS) -o $(BONUS)

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS)
	rm -f $(LIBFT)

re: clean fclean all minishell