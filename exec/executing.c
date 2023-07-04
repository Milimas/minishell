/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 23:51:15 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/04 16:04:00 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_ast_other(t_ast_node *ast_elem)
{
	if (ast_elem && ast_elem->type == PIPE)
		exec_ast_pipe(ast_elem);
	else if (ast_elem && ast_elem->type == AND)
		exec_ast_and(ast_elem);
	else if (ast_elem && ast_elem->type == OR)
		exec_ast_or(ast_elem);
}

static char	*check_cmd(char **paths, char *cmd)
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
	return (NULL);
}

void	update_(t_ast_node *ast_elem)
{
	char	**paths;
	char	*tmp;
	char	*cmd_path;

	paths = get_paths();
	cmd_path = check_cmd(paths, ast_elem->content->cmd->args[0]);
	free_split(paths);
	if (!cmd_path)
		return ;
	tmp = ft_strjoin("_=", cmd_path);
	free(cmd_path);
	ex_modify(tmp);
	free(tmp);
}

int	exec_ast(t_ast_node *ast_elem, enum e_node_type parent_type)
{
	if (ast_elem && ast_elem->type == SUB)
		exec_sub(ast_elem);
	else if (ast_elem && ast_elem->type == CMD && ast_elem->content)
	{
		if (!here_doc(ast_elem->content->cmd->redir->head,
				ast_elem->content->cmd))
			return (0);
		signal(SIGQUIT, sig_quit_handler);
		g_data.pid = fork();
		if (g_data.pid == -1)
			return (ft_putendl_fd(
					"bash: fork: Resource temporarily unavailable", 2), 1);
		if (!g_data.pid)
			exec_cmd(ast_elem);
		if (parent_type != PIPE)
			update_status();
		update_(ast_elem);
		signal(SIGQUIT, SIG_IGN);
	}
	else
		exec_ast_other(ast_elem);
	return (1);
}

void	builts(t_cmd *cmd)
{
	if (!ft_strncmp("exit", cmd->args[0], 5))
		bsh_exit(cmd);
	else if (!ft_strncmp("echo", cmd->args[0], 5))
		bsh_echo(cmd);
	else if (!ft_strncmp("pwd", cmd->args[0], 4))
		bsh_pwd(cmd->fd.out);
	else if (!ft_strncmp("cd", cmd->args[0], 3))
		bsh_cd(cmd);
	else if (!ft_strncmp("export", cmd->args[0], 7))
		bsh_export(cmd);
	else if (!ft_strncmp("unset", cmd->args[0], 6))
		bsh_unset(cmd);
	else if (!ft_strncmp("env", cmd->args[0], 4))
		bsh_env(cmd->fd.out);
	else
		return ;
}
