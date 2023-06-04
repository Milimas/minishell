/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:43:05 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/06/04 21:18:40 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void		sig_ign_handler(int signum);

void	bash_promt(void)
{
	char			*line;
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
		//tests
		g_data.pid = fork();
		if (!g_data.pid)
			exec(lexer, ast);
		waitpid(g_data.pid, NULL, 0);
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
	init_global_data();
	while (1)
	{
		signal(SIGINT, sig_ign_handler);
		signal(SIGQUIT, SIG_IGN);
		bash_promt();
	}
}
