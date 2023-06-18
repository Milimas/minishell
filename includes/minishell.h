/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:02:40 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/17 14:00:57 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
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
typedef struct s_linkedlist	t_linkedlist;
typedef struct s_env		t_env;

enum e_node_type
{
	CMD,
	PIPE,
	AND,
	OR,
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
	ENV = '$',
	PIPE_LINE = '|',
	REDIRECTION_IN = '<',
	REDIRECTION_OUT = '>',
	HERE_DOC,
	DOUBLE_REDIRECTION_OUT,
	WILDCARD = '*',
	PARENTASIS_OPEN = '(',
	PARENTASIS_CLOSE = ')',
	DOUBLE_AMPERSAND = -2,
	LOGICAL_OR_OPERATOR = -3,
};

typedef struct s_data
{
	int			exit_status;
	int			pid;
	char		**envp;
	t_env		*env;
}	t_data;

t_data						g_data;

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

typedef struct s_env
{
	char			*key;
	char			*value;
	t_env			*next;
}	t_env;

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
void			lexer_double_pipe(t_linkedlist *list,
					char **line, int state);
void			lexer_double_ampersand(t_linkedlist *list,
					char **line, int state);
void			lexer_quotes(t_linkedlist *list, char **line, int *state);
void			lexer_wildcard(t_linkedlist *list, t_elem *elem, int state);
int				check_syntax(t_elem *elem);

// parser
void			ft_parser(t_elem *elem, t_ast_node **ast);
t_cmd			*create_cmd(t_elem **elem);
int				count_args(t_elem *elem);
t_cmd			*create_cmd(t_elem **elem);
t_redir_elem	*create_redir(t_elem **elem);
void			append_redir(t_redir_list *redir, t_redir_elem *new);
void			init_global_data(void);
t_env			*envlast(t_env *env);
void			envadd_back(t_env **env, t_env *new);
t_env			*envnew(char *value);
int				is_redirection(t_elem *elem);
int				is_quote(t_elem *elem);
int				is_in_quote(t_elem *elem);
int				is_logical_operator(t_elem *elem);
int				is_ast_logical(t_ast_node *ast);

// builtin
void			bsh_pwd(void);
void			bsh_echo(t_cmd *cmd);
void			bsh_exit(t_cmd *cmd);
void			bsh_cd(t_cmd *cmd);
void			bsh_export(t_cmd *cmd);
void			bsh_unset(t_cmd *cmd);
void			bsh_env(void);

// execution
void			exevc(t_cmd *cmd);
void			exec_ast(t_ast_node *ast_elem, int wait);
int				is_builts(t_cmd *cmd);
void			builts(t_cmd *cmd);

// utils
int				ft_isspace(char c);
char			*get_env_variable(char *name);
char			*ft_strconcat(char *s1, char *s2);
int				ex_ist(char *cmd);
void			ex_modify(char	*cmd);
int				plus_check(char *args);

// debug tools
void			print_linkedlist(t_linkedlist *list);
void			print_ast(t_ast_node *ast);

// main
void			sig_ign_handler(int signum);

#endif