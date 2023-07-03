/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:43:05 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/03 05:19:58 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	set_ast_pipe(t_ast_node *ast, int in, int out)
{
	int	pipe_fd[2];

	if (ast->type == CMD)
	{
		ast->content->cmd->fd.in = in;
		ast->content->cmd->fd.out = out;
	}
	else if (ast->type == PIPE)
	{
		if (pipe(pipe_fd))
		{
			perror("bash");
			return ;
		}
		set_ast_pipe(ast->content->pipe->first, in, pipe_fd[1]);
		set_ast_pipe(ast->content->pipe->second, pipe_fd[0], out);
	}
	else if (ast->type != SUB)
	{
		set_ast_pipe(ast->content->pipe->first, in, out);
		set_ast_pipe(ast->content->pipe->second, in, out);
	}
	else
		set_ast_pipe(ast->content->ast, in, out);
}

void	close_ast_pipe(t_ast_node *ast, int in, int out)
{
	if (ast->type == CMD)
	{
		if (ast->content->cmd->fd.in != in)
			close(ast->content->cmd->fd.in);
		if (ast->content->cmd->fd.out != out)
			close(ast->content->cmd->fd.out);
	}
	else if (ast->type == PIPE)
	{
		close_ast_pipe(ast->content->pipe->first, in, out);
		close_ast_pipe(ast->content->pipe->second, in, out);
	}
	else if (ast->type != SUB)
	{
		close_ast_pipe(ast->content->pipe->first, in, out);
		close_ast_pipe(ast->content->pipe->second, in, out);
	}
	else
		close_ast_pipe(ast->content->ast, in, out);
}

void	execute_line(void)
{
	if (g_data.ast.root && g_data.ast.root->type == CMD
		&& is_builts(g_data.ast.root->content->cmd))
	{
		if (rediring(g_data.ast.root->content->cmd->redir->head,
				g_data.ast.root->content->cmd))
			builts(g_data.ast.root->content->cmd);
	}
	else if (g_data.ast.root)
	{
		set_ast_pipe(g_data.ast.root, STDIN_FILENO, STDOUT_FILENO);
		exec_ast(g_data.ast.root, g_data.ast.root->type);
		close_ast_pipe(g_data.ast.root, STDIN_FILENO, STDOUT_FILENO);
	}
}

void	wait_last(void)
{
	int				status;
	pid_t			pid;

	pid = waitpid(-1, &status, 0);
	while (pid != -1)
	{
		if (pid == g_data.pid)
			g_data.exit_status = WEXITSTATUS(status);
		pid = waitpid(-1, &status, 0);
	}
}

void	proccess_line(char *line)
{
	t_elem			*lexer;

	if (!line)
		exit(!!printf("exit\n"));
	if (ft_strlen(line))
	{
		add_history(line);
		ft_lexer(line);
		lexer = g_data.lexer.head;
		if (check_syntax(g_data.lexer.head))
		{
			free_lexer();
			return ;
		}
		while (g_data.lexer.head)
			g_data.ast.root = ft_parser(&g_data.lexer.head, g_data.ast.root);
		g_data.lexer.head = lexer;
		free_lexer();
		execute_line();
		free_tree(g_data.ast.root);
		wait_last();
	}
}

void	bash_promt(void)
{
	char			*line;

	line = readline(PROMPT_TEXT);
	signal(SIGINT, SIG_IGN);
	proccess_line(line);
	reset_global_data();
	free(line);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	init_global_data();
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_ign_handler);
		bash_promt();
	}
}
