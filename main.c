/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:43:05 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/03 09:50:37 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	execute_line(void)
{
	if (g_data.ast.root && g_data.ast.root->type == CMD
		&& is_builts(g_data.ast.root->content->cmd))
	{
		if (rediring(g_data.ast.root->content->cmd->redir->head,
				g_data.ast.root->content->cmd))
		{
			here_doc(g_data.ast.root->content->cmd->redir->head,
				g_data.ast.root->content->cmd);
			builts(g_data.ast.root->content->cmd);
		}
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

	pid = waitpid(-1, &status, WUNTRACED);
	while (pid != -1)
	{
		if (pid == g_data.pid)
		{
			if (WIFEXITED(status))
				g_data.exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_data.exit_status = WTERMSIG(status) + 128;
		}
		pid = waitpid(-1, &status, WUNTRACED);
	}
}

void	proccess_line(char *line)
{
	t_elem			*lexer;

	if (!line)
		exit(!printf("exit\n"));
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
	g_data.envp = envp;
	init_global_data();
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_ign_handler);
		bash_promt();
	}
	return (0);
}
