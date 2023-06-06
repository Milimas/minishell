/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 23:42:59 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/05 19:44:09 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_paths(void)
{
	char	**envp;

	envp = g_data.envp;
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (ft_split(*envp + 5, ':'));
		envp++;
	}
	return (NULL);
}

char	*cmd_file(char **paths, char *cmd)
{
	char	*path_tmp;
	char	*file;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (*paths && cmd)
	{
		path_tmp = ft_strjoin(*paths, "/");
		file = ft_strjoin(path_tmp, cmd);
		free(path_tmp);
		if (access(file, X_OK) == 0)
			return (file);
		free(file);
		paths++;
	}
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found", 2);
	ft_putstr_fd("\n", 2);
	exit(127);
	return (NULL);
}

int	is_builts(t_cmd *cmd)
{
	if (!ft_strncmp("exit", cmd->args[0], 5))
		return (1);
	else if (!ft_strncmp("echo", cmd->args[0], 5))
		return (1);
	else if (!ft_strncmp("pwd", cmd->args[0], 4))
		return (1);
	else if (!ft_strncmp("cd", cmd->args[0], 3))
		return (1);
	else if (!ft_strncmp("export", cmd->args[0], 7))
		return (1);
	else
		return (0);
}

void	builts(t_cmd *cmd)
{
	if (!ft_strncmp("exit", cmd->args[0], 5))
		bsh_exit(cmd);
	else if (!ft_strncmp("echo", cmd->args[0], 5))
		bsh_echo(cmd);
	else if (!ft_strncmp("pwd", cmd->args[0], 4))
		bsh_pwd();
	else if (!ft_strncmp("cd", cmd->args[0], 3))
		bsh_cd(cmd);
	else if (!ft_strncmp("export", cmd->args[0], 7))
		bsh_export(cmd);
	else
		return ;
}

void	exec(t_cmd *cmd)
{
	execve(cmd_file(get_paths(), cmd->args[0]), cmd->args, g_data.envp);
	exit(g_data.exit_status);
}

/*
* had function ktkhdam recursive batexecuti ga3 les command li f tree
* ra ktb9a tsarkal ftree ila l9at pipe ktexecuti ga3 commands li fpipe 
* ila l9at command ra ktexecutiha
*/
void	exec_ast(t_ast_node *ast_elem)
{
	if (ast_elem && ast_elem->type == CMD && ast_elem->content)
	{
		if (is_builts(ast_elem->content->cmd))
			builts(ast_elem->content->cmd);
		else
		{
			g_data.pid = fork();
			if (!g_data.pid)
			{
				signal(SIGQUIT, SIG_IGN);
				signal(SIGINT, sig_ign_handler);
				exec(ast_elem->content->cmd);
			}
			waitpid(g_data.pid, NULL, 0);
		}
	}
	else if (ast_elem && ast_elem->type == PIPE)
	{
		exec_ast(ast_elem->content->pipe->first);
		exec_ast(ast_elem->content->pipe->second);
	}
}
