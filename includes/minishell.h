/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:19:32 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/11 07:29:44 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <errno.h>
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
	SUB,
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
	t_pipe		*pipe;
	t_cmd		*cmd;
	t_ast_node	*ast;
}	t_union;

typedef struct s_env
{
	char			*key;
	char			*value;
	t_env			*next;
}	t_env;

typedef struct s_data
{
	int				exit_status;
	int				pid;
	int				subpid;
	t_env			*env;
	t_ast			ast;
	t_linkedlist	lexer;
}	t_data;

t_data						g_data;

// list.c
int				list_is_empty(t_linkedlist *list);
t_linkedlist	*list_init(t_linkedlist *list);
t_elem			*list_new_elem(char *content,
					int len, enum e_token type, enum e_state state);
void			list_add_back(t_linkedlist *list, t_elem *new);
void			list_remove(t_linkedlist *list, t_elem *node);
void			free_lexer(void);
void			free_tree(t_ast_node *ast);
void			free_split(char **split);

// lexer
t_linkedlist	*ft_lexer(char *line);
int				is_token(char c);
int				is_double_redirection(char *line);
void			lexer_escape(t_linkedlist *list, char **line, int state);
void			lexer_redirection(t_linkedlist *list,
					char **line, int state);
void			lexer_double_pipe(t_linkedlist *list,
					char **line, int state);
void			lexer_double_ampersand(t_linkedlist *list,
					char **line, int state);
void			lexer_quotes(t_linkedlist *list, char **line, int *state);
void			lexer_wildcard(t_linkedlist *list, t_elem *elem, int state);
int				word_len(char *line);
int				env_len(char *line);
int				filename_len(char *line);
void			delete_from_list(t_linkedlist **list, t_elem *elem);
void			lexer_env(t_linkedlist *list, char **line, int state);
// syntax
int				check_syntax(t_elem *elem);
int				syntax_error(t_elem *elem, int estatus);
int				redirection_syntax(t_elem *elem);
int				quote_syntax(t_elem **elem);
int				logical_syntax(t_elem *elem);
int				parentasis_syntax(t_elem *elem);
int				is_type_state(t_elem *elem, enum e_token type,
					enum e_state state);
int				ambiguous_redirect(t_elem *elem, int estatus);

// wildcard
int				match_f(char *d_name, char *pattern);
int				is_regular_file(const char *path);
void			lexer_wildcard(t_linkedlist *list, t_elem *elem, int state);
void			sort_list(t_list *list);
t_list			*get_files(char *pattern);
void			get_files_rec_helper(char *path, char *pattern,
					t_list **list, struct dirent *dir);
void			get_files_rec(char *path, char *pattern, t_list **list);
char			*skip_currdir(char *path);
void			sort_list(t_list *list);
int				is_hidden(char *d_name, char *pattern);
char			*join_path(char *path, char *d_name, char *sep);

// parser
t_ast_node		*ft_parser(t_elem **elem, t_ast_node *ast);
int				count_args(t_elem *elem);
t_ast_node		*create_cmd(t_elem **elem);
t_redir_elem	*create_redir(t_elem **elem);
void			append_redir(t_redir_list *redir, t_redir_elem *new);
void			init_global_data(void);
void			reset_global_data(void);
t_env			*envlast(t_env *env);
void			envadd_back(t_env **env, t_env *new);
t_env			*envnew(char *value);
int				is_redirection(t_elem *elem);
int				is_quote(t_elem *elem);
int				is_in_quote(t_elem *elem);
int				is_logical_operator(t_elem *elem);
int				is_ast_logical(t_ast_node *ast);

// builtin
void			bsh_pwd(int fd);
void			bsh_echo(t_cmd *cmd);
void			bsh_exit(t_cmd *cmd);
void			bsh_cd(t_cmd *cmd);
void			bsh_export(t_cmd *cmd);
void			bsh_unset(t_cmd *cmd);
void			bsh_env(int fd);
void			env_modo(char *str);

// builtin utils
int				ft_envsize(t_env *lst);
void			exp_tab(t_env *env, int fd);
void			sort_chartab(char **tab, int size);
void			print_export(char **tab, int size, int fd);
void			built_error(char *args);
int				key_len(char *cmd, char *env);

// execution
char			**envp_totab(void);
char			**get_paths(void);
char			*cmd_file(char **paths, char *cmd);
void			update_status(pid_t pid);
void			exec_sub(t_ast_node *ast_elem);
void			exevc(t_cmd *cmd);
void			exec_cmd(t_ast_node *ast_elem);
void			exec_ast_pipe(t_ast_node *ast_elem);
void			exec_ast_or(t_ast_node *ast_elem);
void			exec_ast_and(t_ast_node *ast_elem);
int				exec_ast(t_ast_node *ast_elem, enum e_node_type parent_type);
int				is_builts(t_cmd *cmd);
void			builts(t_cmd *cmd);

//heredoc
void			putfilefd(char *av, int fd);
int				rediring(t_redir_elem *redir, t_cmd *cmd);
char			*rm_quotes(char *str);
char			*out_quotes(int len, char *str, int nb, char *res);
void			expanding(char *str, int fd, char *limiter);
int				here_doc(t_redir_elem *redir, t_cmd *cmd);
char			which_quote(char *str);
int				check_limiter(char *str);

// utils
int				ft_isspace(char c);
char			*get_env_variable(char *name);
char			*ft_strconcat(char *s1, char *s2);
int				ex_ist(char *cmd);
void			ex_modify(char	*cmd);
int				plus_check(char *args);
int				in_env(char *key, char *pwd);
void			set_oldpwd(void);
void			set_pwd(void);
void			free_env(t_env *env);
int				check_permission(char *arg);
int				env_size(t_env *lst);

// debug tools
void			print_lexer(t_linkedlist *list);
void			print_ast(t_ast_node *ast);
char			*ast_type(t_ast_node *ast);

// signals
void			sig_quit_handler(int signum);
void			sig_ign_handler(int signum);

// main
void			proccess_line(char *line);
void			close_ast_pipe(t_ast_node *ast, int in, int out);
void			set_ast_pipe(t_ast_node *ast, int in, int out);
void			wait_last(void);

#endif