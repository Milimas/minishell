/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:43:05 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/26 07:21:27 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	execute_line(void)
{
	if (g_data.ast.root->type == CMD
		&& is_builts(g_data.ast.root->content->cmd))
	{
		rediring(g_data.ast.root->content->cmd->redir->head,
			g_data.ast.root->content->cmd);
		builts(g_data.ast.root->content->cmd);
	}
	else
		exec_ast(g_data.ast.root, g_data.ast.root->type);
}

void	proccess_line(char *line)
{
	int				status;

	if (!line)
		exit(!!printf("exit\n"));
	if (ft_strlen(line))
	{
		add_history(line);
		ft_lexer(line);
		if (check_syntax(g_data.lexer.head))
			return ;
		while (g_data.lexer.head)
			g_data.ast.root = ft_parser(&g_data.lexer.head, g_data.ast.root);
		free_lexer();
		execute_line();
		free_tree(g_data.ast.root);
		while (waitpid(-1, &status, 0) != -1)
			if (waitpid(-1, &status, 0) == g_data.pid)
				g_data.exit_status = WEXITSTATUS(status);
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
