/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:43:05 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/05/25 08:39:26 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void		sig_ign_handler(int signum);

char	*state_to_text(int state)
{
	if (state == IN_DOUBLE_QUOTE)
		return ("IN_DOUBLE_QUOTE");
	if (state == IN_QUOTE)
		return ("IN_QUOTE");
	if (state == GENERAL)
		return ("GENERAL");
	if (state == IN_PARENTASIS)
		return ("IN_PARENTASIS");
	return (NULL);
}

char	*type_to_text(int type)
{
	if (type == WORD)
		return ("WORD");
	if (type == WHITE_SPACE)
		return ("WHITE_SPACE");
	if (type == NEW_LINE)
		return ("NEW_LINE");
	if (type == QUOTE)
		return ("QUOTE");
	if (type == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	if (type == ESCAPE)
		return ("ESCAPE");
	if (type == ENV)
		return ("ENV");
	if (type == PIPE_LINE)
		return ("PIPE_LINE");
	if (type == REDIRECTION_IN)
		return ("REDIRECTION_IN");
	if (type == REDIRECTION_OUT)
		return ("REDIRECTION_OUT");
	if (type == HERE_DOC)
		return ("HERE_DOC");
	if (type == DOUBLE_REDIRECTION_OUT)
		return ("DOUBLE_REDIRECTION_OUT");
	if (type == WILDCARD)
		return ("WILDCARD");
	if (type == PARENTASIS_OPEN)
		return ("PARENTASIS_OPEN");
	if (type == PARENTASIS_CLOSE)
		return ("PARENTASIS_CLOSE");
	if (type == DOUBLE_AMPERSAND)
		return ("DOUBLE_AMPERSAND");
	if (type == LOGOCAL_OR_OPERATOR)
		return ("LOGOCAL_OR_OPERATOR");
	return (NULL);
}

void	print_linkedlist(t_linkedlist *list)
{
	t_elem	*elem;

	elem = list->head;
	printf("=================================================================\n");
	while (elem)
	{
		printf("content: %s\tlen: %d\tstate: %s\ttype: %s\n", elem->content, elem->len, state_to_text(elem->state), type_to_text(elem->type));
		elem = elem->next;
	}
	printf("=================================================================\n");
}

int		count_args(t_elem *elem)
{
	int	count;

	count = 0;
	while (elem && elem->type != PIPE_LINE)
	{
		elem = elem->next;
		count++;
	}
	return (count);
}

t_redir_elem	*create_redir(t_elem **elem)
{
	t_redir_elem	*redir;

	redir = ft_calloc(sizeof(t_redir_elem), 1);
	redir->type = (*elem)->type;
	*elem = (*elem)->next;
	while ((*elem) && (*elem)->type == WHITE_SPACE)
		*elem = (*elem)->next;
	if ((*elem) && (*elem)->type == WORD)
	{
		redir->arg = "";
		if ((*elem)->state != GENERAL)
			while ((*elem) && (*elem)->state != GENERAL)
			{
				redir->arg = ft_strjoin(redir->arg, (*elem)->content);
				(*elem) = (*elem)->next;
			}
		else
			redir->arg = (*elem)->content;
		if ((*elem))
			(*elem) = (*elem)->next;
	}
	else
		printf("Error: redir");
	return (redir);
}

void	append_redir(t_redir_list *redir, t_redir_elem *new)
{
	if (!new)
		return ;
	if (!redir->head)
		redir->head = new;
	else
		redir->tail->next = new;
	redir->tail = new;
	redir->size++;
}

t_cmd	*create_cmd(t_elem **elem)
{
	t_cmd	*cmd;
	char	**tmp_args;

	cmd = ft_calloc(sizeof(t_cmd), 1);
	cmd->args = ft_calloc(sizeof(char *), count_args((*elem)) + 1);
	tmp_args = cmd->args;
	cmd->redir = ft_calloc(sizeof(t_redir_list), 1); // fill this only if there is a redirection else set it to null
	while ((*elem))
	{
		if ((*elem) && ((*elem)->type == REDIRECTION_IN || (*elem)->type == REDIRECTION_OUT || (*elem)->type == HERE_DOC || (*elem)->type == DOUBLE_REDIRECTION_OUT))
			append_redir(cmd->redir, create_redir(elem));
		if ((*elem) && ((*elem)->type != WHITE_SPACE))
			*tmp_args++ = ft_strdup((*elem)->content);
		if ((*elem))
			(*elem) = (*elem)->next;
		if ((*elem) && (*elem)->type == PIPE_LINE && (*elem)->state == GENERAL)
			break;
	}
	cmd->fd.in = 0;
	cmd->fd.out = 1;
	return (cmd);
}

void	print_ast(t_ast_node *ast)
{
	char			**args;
	t_redir_elem	*redir;

	if (ast && ast->type == CMD)
	{
		args = ast->content->cmd->args;
		printf("cmd: %s\n", *args);
		while (*++args)
		{
			printf("arg: %s\n", *args);
		}
		if (ast->content->cmd->redir && ast->content->cmd->redir->head)
		{
			redir = ast->content->cmd->redir->head;
			while (redir)
			{
				printf("redir: %s %d\n", redir->arg, redir->type);
				redir = redir->next;
			}
		}
		else
		{
			printf("no redir\n");
		}
	}
	if (ast && ast->type == PIPE)
	{
		print_ast(ast->content->pipe->first);
		printf("--------------------------------\n");
		print_ast(ast->content->pipe->second);
	}
}

void	bash_promt(void)
{
	char	*line;
	t_linkedlist	*lexer;
	t_ast			ast;

	line = readline(PROMPT_TEXT);
	if (!line)
		exit(!!printf("exit\n"));
	if (ft_strlen(line))
	{
		add_history(line);
		lexer = ft_lexer(line);
		print_linkedlist(lexer);
		ast.root = NULL;
		ft_parser(lexer->head, &ast.root);
		print_ast(ast.root);
	}
	free(line);
}

void	sig_ign_handler(int signum)
{
	(void)signum;
	rl_on_new_line();
	write(1, "\n", 1);
	rl_replace_line("", 1);
	rl_redisplay();
}

void	disable_sigint_char(void)
{
	struct termios	t;

	tcgetattr(STDIN_FILENO, &t);
	t.c_cflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void	enable_sigint_char(void)
{
	struct termios	t;

	tcgetattr(STDIN_FILENO, &t);
	t.c_cflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	disable_sigint_char();
	// enable_sigint_char();
	while (1)
	{
		signal(SIGINT, sig_ign_handler);
		signal(SIGQUIT, SIG_IGN);
		bash_promt();
	}
}
