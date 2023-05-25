#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include "libft.h"

# define PROMPT_TEXT "\033[0;31mbash-0.1$ \033[0m"

extern char					**environ;

typedef struct s_pipe		t_pipe;
typedef struct s_cmd		t_cmd;
typedef union u_union		t_union;
typedef struct s_ast		t_ast;
typedef struct s_ast_node	t_ast_node;
typedef struct s_fd			t_fd;
typedef struct s_redir_list	t_redir_list;
typedef struct s_redir_elem	t_redir_elem;
typedef struct s_elem		t_elem;
typedef struct s_linkedlist		t_linkedlist;

enum e_node_type
{
	CMD,
	PIPE,
};

enum e_state
{
	IN_DOUBLE_QUOTE = '"',
	IN_QUOTE = '\'',
	GENERAL,
	IN_PARENTASIS,
};

enum e_token
{
	WORD = -1,
	WHITE_SPACE = ' ',
	NEW_LINE = '\n',
	QUOTE = '\'',
	DOUBLE_QUOTE = '\"',
	ESCAPE = '\\',
	ENV = '$',
	PIPE_LINE = '|',
	REDIRECTION_IN = '<',
	REDIRECTION_OUT = '>',
	HERE_DOC,
	DOUBLE_REDIRECTION_OUT,
	WILDCARD = '*',
	PARENTASIS_OPEN = '(',
	PARENTASIS_CLOSE = ')',
	DOUBLE_AMPERSAND,
	LOGOCAL_OR_OPERATOR,
};

typedef struct s_linkedlist
{
	t_elem		*head;
	t_elem		*tail;
	int			size;
}	t_linkedlist;

typedef struct s_elem
{
	char			*content;
	int				len;
	enum e_token	type;
	enum e_state	state;
	t_elem			*next;
	t_elem			*prev;
}	t_elem;

typedef struct s_ast_node
{
	enum e_node_type	type;
	t_union				*content;
}	t_ast_node;

typedef struct s_ast
{
	t_ast_node	*root;	
}	t_ast;

typedef struct s_pipe
{
	t_ast_node	*first;
	t_ast_node	*second;
}	t_pipe;

typedef struct s_fd
{
	int	in;
	int	out;
}	t_fd;

typedef struct s_cmd
{
	char			**args;
	t_fd			fd;
	t_redir_list	*redir;
}	t_cmd;

typedef struct s_redir_list
{
	t_redir_elem	*head;
	t_redir_elem	*tail;
	int				size;
}	t_redir_list;

typedef struct s_redir_elem
{
	char			*arg;
	enum e_token	type;
	t_redir_elem	*next;
}	t_redir_elem;

typedef union u_union
{
	t_pipe	*pipe;
	t_cmd	*cmd;
}	t_union;

// list.c
int				list_is_empty(t_linkedlist *list);
t_linkedlist	*list_init(t_linkedlist *list);
t_elem			*list_new_elem(char *content,
						int len, enum e_token type, enum e_state state);
void			list_add_back(t_linkedlist *list, t_elem *new);
void			list_free(t_linkedlist *list);

// lexer
t_linkedlist	*ft_lexer(char *line);
int				is_token(char c);
int				is_double_redirection(char *line);
void			lexer_escape(t_linkedlist *list, char **line, int state);
void			lexer_double_redirection(t_linkedlist *list,
					char **line, int state);
void			lexer_quotes(t_linkedlist *list, char **line, int *state);

// parser
void			ft_parser(t_elem *elem, t_ast_node **ast);
t_cmd			*create_cmd(t_elem **elem);

// builtin
void			bsh_pwd(void);
void			bsh_echo(t_elem *lexer);
void			bsh_chdir(t_cmd *cmd);

// utils
int				ft_isspace(char c);
char			*get_env_variable(char *name);

#endif