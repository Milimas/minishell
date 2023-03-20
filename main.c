/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:43:05 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/03/19 23:14:35 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <termios.h>
#include "includes/libft.h"

#define PROMPT_TEXT "\033[0;31mbash-0.1$ \033[0m"

extern char	**environ;

typedef struct s_cmd
{
	char	**args;
	char	*cmd;
}	t_cmd;

void		sig_ign_handler(int signum);

char	*get_env_variable(char *name)
{
	char	**env;

	env = environ;
	while (*env)
	{
		if (!ft_strncmp(name, *env, ft_strlen(name))
			&& (*env)[ft_strlen(name)] == '=')
			return (ft_strchr(*env, '=') + 1);
		env++;
	}
	return (NULL);
}

void	bsh_echo(t_cmd *cmd)
{
	int		n;
	char	*flag;

	n = 1;
	cmd->args++;
	while (*cmd->args && ft_strnstr(*cmd->args, "-", ft_strlen(*cmd->args) + 1))
	{
		flag = (*cmd->args);
		while (++flag && *flag)
		{
			if (!ft_strchr("neE", *flag))
			{
				flag = NULL;
				break ;
			}
		}
		if (!flag)
			break ;
		cmd->args++;
		n = 0;
	}
	while (*cmd->args)
	{
		ft_putstr_fd(*cmd->args, 1);
		cmd->args++;
		write(1, " ", !!(*cmd->args));
	}
	write(1, "\n", 1 * n);
}

void	bsh_chdir(t_cmd *cmd)
{
	char	*dir;
	DIR		*dirp;

	dir = *(++cmd->args);
	if (!cmd->args)
	{
		chdir(NULL);
		return ;
	}
	dirp = opendir(*cmd->args);
	if (!dirp)
		perror("bash: cd");
	else
		closedir(dirp);
	chdir(dir);
}

void	bsh_pwd(void)
{
	char *buf;

	buf = getcwd(NULL, 10);
	printf("%s\n", buf);
	free(buf);
}

void	bsh_env(void)
{
	char	**env;

	env = environ;
	while (*env)
	{
		if (ft_strchr(*env, '='))
			printf("%s\n", *env);
		env++;
	}	
}

static size_t	bsh_wordcount(char const *s, char c)
{
	int	wc;

	wc = 0;
	while (*s)
	{
		while (*s && !ft_memcmp(s, &c, 1))
			s++;
		if (*s)
		{
			if (*s == '"' && ft_strchr(s + 1, '"'))
				s = (ft_strchr(s + 1, '"')) + 1;
			if (*s == '\'' && ft_strchr(s + 1, '\''))
				s = (ft_strchr(s + 1, '\'')) + 1;
			wc++;
		}
		while (*s && ft_memcmp(s, &c, 1))
			s++;
	}
	return (wc);
}

char	**bsh_split(char const *s, char c)
{
	char	**arr;
	char	**a;
	size_t	size;

	if (!s)
		return (NULL);
	size = bsh_wordcount(s, c) + 1;
	arr = (char **)malloc(size * sizeof(char *));
	if (!arr)
		return (NULL);
	a = arr;
	while (*s)
	{
		while (*s && !ft_memcmp(s, &c, 1))
			s++;
		if (*s == '"' && ft_strchr(s + 1, '"'))
		{
			*a++ = ft_substr(s, 1, ft_strrchr(s, '"') - s - 1);
			s = ft_strchr(s + 1, '"') + 1;
		}
		if (*s == '\'' && ft_strchr(s + 1, '\''))
		{
			*a++ = ft_substr(s, 1, ft_strrchr(s, '\'') - s - 1);
			s = ft_strchr(s + 1, '\'') + 1;
		}
		if (*s && ft_strchr(s, c))
			*a++ = ft_substr(s, 0, ft_strchr(s, c) - s);
		else if (*s)
			*a++ = ft_substr(s, 0, ft_strchr(s, 0) - s);
		while (*s && ft_memcmp(s, &c, 1))
			s++;
	}
	*a = 0;
	return (arr);
}

t_cmd	parse_command(char *line)
{
	t_cmd	cmd;
	char	**tmp_args;
	char	*tmp;

	tmp_args = bsh_split(line, ' ');
	cmd.args = tmp_args;
	while (*tmp_args)
	{
		if (ft_strchr(*tmp_args, '$'))
		{
			tmp = ft_strchr(*tmp_args, '$');
			tmp = ft_substr(tmp, 1, ft_strchr(tmp, ' ') - tmp);
			
			// printf("====>%s\n", get_env_variable(tmp));
		}
		tmp_args++;
	}
	cmd.cmd = cmd.args[0];
	return (cmd);
}

void	bash_promt(void)
{
	char	*line;
	t_cmd	cmd;

	line = readline(PROMPT_TEXT);
	if (line && *line != 0)
	{
		add_history(line);
		cmd = parse_command(line);
		if (cmd.cmd && !ft_strncmp(cmd.cmd, "echo", ft_strlen(cmd.cmd)))
			bsh_echo(&cmd);
		if (cmd.cmd && !ft_strncmp(cmd.cmd, "cd", ft_strlen(cmd.cmd)))
			bsh_chdir(&cmd);
		if (cmd.cmd && !ft_strncmp(cmd.cmd, "pwd", ft_strlen(cmd.cmd)))
			bsh_pwd();
		if (cmd.cmd && !ft_strncmp(cmd.cmd, "env", ft_strlen(cmd.cmd)))
			bsh_env();
		if (cmd.cmd && !ft_strncmp(cmd.cmd, "exit", ft_strlen(cmd.cmd)))
			exit(!printf("exit\n"));
		if (cmd.cmd && !ft_strncmp(cmd.cmd, "dbg", ft_strlen(cmd.cmd)))
			get_env_variable(cmd.args[1]);
	}
	if (!line)
		exit(!printf("exit\n"));
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
