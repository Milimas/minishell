/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:43:05 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/23 19:43:43 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void		sig_ign_handler(int signum);

void	proccess_line(char *line)
{
	int				status;
	pid_t			wait_pid;

	if (!line)
		exit(!!printf("exit\n"));
	if (ft_strlen(line))
	{
		add_history(line);
		g_data.lexer = *ft_lexer(line);
		print_linkedlist(&g_data.lexer);
		if (check_syntax(g_data.lexer.head))
			return ;
		// while (g_data.lexer.head)
		// {
		printf("===============================================================================================\n");
		while (g_data.lexer.head)
		{
			g_data.ast.root = ft_parser(&g_data.lexer.head, g_data.ast.root);
		}
		print_ast(g_data.ast.root);
		printf("===============================================================================================\n");
		// }
		// print_ast(g_data.ast.root);
		if (g_data.ast.root->type == CMD && is_builts(g_data.ast.root->content->cmd))
		{
			rediring(g_data.ast.root->content->cmd->redir->head, g_data.ast.root->content->cmd);
			builts(g_data.ast.root->content->cmd);
		}
		else
			exec_ast(g_data.ast.root, g_data.ast.root->type);
		wait_pid = waitpid(-1, &status, 0);
		if (wait_pid == g_data.pid)
				g_data.exit_status = WEXITSTATUS(status);
		while (wait_pid != -1)
		{
			wait_pid = waitpid(-1, &status, 0);
			if (wait_pid == g_data.pid)
				g_data.exit_status = WEXITSTATUS(status);
		}
		// print_ast(g_data.ast.root);
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

void	sig_ign_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
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
	// disable_sigint_char();
	// enable_sigint_char();
	init_global_data();
	// putfilefd("mok",1);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_ign_handler);
		bash_promt();
	}
}
