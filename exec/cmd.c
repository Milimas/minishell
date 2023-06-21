/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 23:42:59 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/21 04:16:06 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_paths(void)
{
	t_env	*envp;

	envp = g_data.env;
	while (envp)
	{
		if (ft_strncmp(envp->key, "PATH", 5) == 0)
			return (ft_split(envp->value, ':'));
		envp = envp->next;
	}
	return (NULL);
}

char	*cmd_file(char **paths, char *cmd)
{
	char	*path_tmp;
	char	*file;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (paths && *paths && cmd)
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

void	handle_err(int pid)
{
	if (pid < 0)
	{
		perror("fork error");
		exit(-1);
	}
}

void	exevc(t_cmd *cmd)
{
	execve(cmd_file(get_paths(), cmd->args[0]), cmd->args, NULL);
	exit(g_data.exit_status);
}

void	exec_ast(t_ast_node *ast_elem)
{
	int	pipe_fd[2];
	int	status;

	if (ast_elem && ast_elem->type == SUB)
	{
		g_data.pid = fork();
		if (g_data.pid == -1)
		{
			ft_putendl_fd("bash: fork: Resource temporarily unavailable", 2);
			return ;
		}
		if (!g_data.pid)
		{
			exec_ast(ast_elem->content->ast);
			exit(g_data.exit_status);
		}
	}
	if (ast_elem && ast_elem->type == CMD && ast_elem->content)
	{
		g_data.pid = fork();
		if (g_data.pid == -1) {
			ft_putendl_fd("bash: fork: Resource temporarily unavailable", 2);
			return ;
		}
		if (!g_data.pid)
		{
			signal(SIGQUIT, SIG_IGN);
			signal(SIGINT, sig_ign_handler);
			dup2(ast_elem->content->cmd->fd.in, STDIN_FILENO);
			dup2(ast_elem->content->cmd->fd.out, STDOUT_FILENO);
			close(g_data.first_pipe);
			if (is_builts(ast_elem->content->cmd))
				builts(ast_elem->content->cmd);
			else
				exevc(ast_elem->content->cmd);
			exit(g_data.exit_status);
		}
		waitpid(g_data.pid, &status, 0);
		g_data.exit_status = WEXITSTATUS(status);
		if (ast_elem->content->cmd->fd.out != STDOUT_FILENO)
			close(ast_elem->content->cmd->fd.out);
		if (ast_elem->content->cmd->fd.in != STDIN_FILENO)
			close(ast_elem->content->cmd->fd.in);
	}
	else if (ast_elem && is_ast_logical(ast_elem))
	{
		if (pipe(pipe_fd) == -1)
			return ;
		g_data.first_pipe = pipe_fd[0];
		if (ast_elem->type == PIPE)
		{
			if (ast_elem->content->pipe->first->type == CMD)
				ast_elem->content->pipe->first->content->cmd->fd.out = pipe_fd[1];
			if (ast_elem->content->pipe->second->type == CMD)
				ast_elem->content->pipe->second->content->cmd->fd.in = pipe_fd[0];
			if (ast_elem->content->pipe->second->type == PIPE)
				ast_elem->content->pipe->second->content->pipe->first->content->cmd->fd.in = pipe_fd[0];
		}
		exec_ast(ast_elem->content->pipe->first);
		if (ast_elem->type == AND && g_data.exit_status)
			return ;
		if (ast_elem->type == OR && !g_data.exit_status)
			return ;
		exec_ast(ast_elem->content->pipe->second);
	}
}
