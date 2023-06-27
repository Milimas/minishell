/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 23:42:59 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/27 19:30:53 by abeihaqi         ###   ########.fr       */
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
		{
			return (ft_split(envp->value, ':'));
		}
		envp = envp->next;
	}
	return (NULL);
}

char	*cmd_file(char **paths, char *cmd)
{
	char	*path_tmp;
	char	*file;

	if (*cmd && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (paths && *paths && cmd && *cmd)
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
	char	**paths;
	char	*cmd_path;

	paths = get_paths();
	cmd_path = cmd_file(paths, cmd->args[0]);
	free_split(paths);
	execve(cmd_path, cmd->args, NULL);
	free(cmd_path);
	perror("bash");
	exit(g_data.exit_status);
}

void	exec_ast_pipe(t_ast_node *ast_elem)
{
	int	pipe_fd[2];

	if (!ast_elem->content->pipe->first || !ast_elem->content->pipe->second)
		return ;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return ;
	}
	g_data.first_pipe = pipe_fd[0];
	if (ast_elem->type == PIPE) // ila kano ba9in andk pipes l9dam anormiw bihom hado
	{
		if (ast_elem->content->pipe->first->type == PIPE || ast_elem->content->pipe->first->type == OR || ast_elem->content->pipe->first->type == AND)
			if (ast_elem->content->pipe->first->type != SUB)
				ast_elem->content->pipe->first->content->pipe->second->content->cmd->fd.out = pipe_fd[1];
		if (ast_elem->content->pipe->first->type == CMD)
			ast_elem->content->pipe->first->content->cmd->fd.out = pipe_fd[1];
		if (ast_elem->content->pipe->second->type == CMD)
			ast_elem->content->pipe->second->content->cmd->fd.in = pipe_fd[0];
	}
	exec_ast(ast_elem->content->pipe->first, ast_elem->type);
	close(pipe_fd[1]);
	exec_ast(ast_elem->content->pipe->second, ast_elem->type);
	close(pipe_fd[0]);
}

void	exec_ast_or(t_ast_node *ast_elem)
{
	if (ast_elem->content->pipe->first)
		exec_ast(ast_elem->content->pipe->first, ast_elem->type);
	if (g_data.exit_status && ast_elem->content->pipe->second)
		exec_ast(ast_elem->content->pipe->second, ast_elem->type);
}

void	exec_ast_and(t_ast_node *ast_elem)
{
	if (ast_elem->content->pipe->first)
		exec_ast(ast_elem->content->pipe->first, ast_elem->type);
	if (!g_data.exit_status && ast_elem->content->pipe->second)
		exec_ast(ast_elem->content->pipe->second, ast_elem->type);
}

void	exec_cmd(t_ast_node *ast_elem)
{
	signal(SIGINT, SIG_DFL);
	if (ast_elem->content->cmd->fd.in != STDIN_FILENO)
		close(ast_elem->content->cmd->fd.in + 1);
	rediring(ast_elem->content->cmd->redir->head, ast_elem->content->cmd);
	signal(SIGQUIT, sig_quit_handler);
	if (dup2(ast_elem->content->cmd->fd.in, STDIN_FILENO) == -1)
		perror("dup2: stdin");
	if (dup2(ast_elem->content->cmd->fd.out, STDOUT_FILENO) == -1)
		perror("dup2: stdout");
	close(g_data.first_pipe);
	g_data.exit_status = 0;
	if (is_builts(ast_elem->content->cmd))
		builts(ast_elem->content->cmd);
	else if (ast_elem->content->cmd->args[0])
		exevc(ast_elem->content->cmd);
	exit(g_data.exit_status);
}

void	update_status(void)
{
	int	status;

	waitpid(g_data.pid, &status, 0);
	g_data.exit_status = WEXITSTATUS(status);
}

void	exec_sub(t_ast_node *ast_elem)
{
	g_data.pid = fork();
	if (g_data.pid == -1)
	{
		ft_putendl_fd("bash: fork: Resource temporarily unavailable", 2);
		return ;
	}
	if (!g_data.pid)
	{
		exec_ast(ast_elem->content->ast, ast_elem->type);
		exit(g_data.exit_status);
	}
	update_status();
}

void	exec_ast(t_ast_node *ast_elem, enum e_node_type parent_type)
{
	if (ast_elem && ast_elem->type == SUB)
		exec_sub(ast_elem);
	else if (ast_elem && ast_elem->type == CMD && ast_elem->content)
	{
		if (is_builts(ast_elem->content->cmd) && parent_type != PIPE)
		{
			builts(ast_elem->content->cmd);
			return ;
		}
		g_data.pid = fork();
		if (g_data.pid == -1)
		{
			ft_putendl_fd("bash: fork: Resource temporarily unavailable", 2);
			return ;
		}
		if (!g_data.pid)
			exec_cmd(ast_elem);
		if (parent_type != PIPE)
			update_status();
	}
	else if (ast_elem && ast_elem->type == PIPE)
		exec_ast_pipe(ast_elem);
	else if (ast_elem && ast_elem->type == AND)
		exec_ast_and(ast_elem);
	else if (ast_elem && ast_elem->type == OR)
		exec_ast_or(ast_elem);
}
