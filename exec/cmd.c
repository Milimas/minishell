/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 23:42:59 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/11 23:49:57 by abeihaqi         ###   ########.fr       */
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
	if (!cmd->args[0])
		return (0);
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
	else if (!ft_strncmp("unset", cmd->args[0], 6))
		return (1);
	else if (!ft_strncmp("env", cmd->args[0], 4))
		return (1);
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
	else if (!ft_strncmp("unset", cmd->args[0], 6))
		bsh_unset(cmd);
	else if (!ft_strncmp("env", cmd->args[0], 4))
		bsh_env();
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
void	exec_ast(t_ast_node *ast_elem, int wait)
{
	int	status;
	
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
		}
	}
	else if (ast_elem && is_ast_logical(ast_elem))
	{
		exec_ast(ast_elem->content->pipe->first, 1);
		if (ast_elem->type == AND && g_data.exit_status)
			return ;
		if (ast_elem->type == OR && !g_data.exit_status)
			return ;
		exec_ast(ast_elem->content->pipe->second, 1);
	}
	if (wait)
	{
		waitpid(g_data.pid, &status, 0);
		g_data.exit_status = WEXITSTATUS(status);
	}
}
