/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 23:42:59 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/04 11:58:19 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_err(int pid)
{
	if (pid < 0)
	{
		perror("fork error");
		exit(-1);
	}
}

int	env_size(t_env *lst)
{
	int		size;
	t_env	*current;

	size = 0;
	current = lst;
	while (current != NULL)
	{
		current = current->next;
		size++;
	}
	return (size);
}

char	**envp_totab(void)
{
	char	**tab;
	int		size;
	t_env	*env;
	char	*tmp;
	int		i;

	i = 0;
	env = g_data.env;
	size = env_size(env);
	tab = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!tab)
		return (NULL);
	while (env && i < size)
	{
		if (env->value)
		{
			tmp = ft_strjoin(env->key, "=");
			tab[i] = ft_strjoin(tmp, env->value);
		}
		else
			tab[i] = ft_strdup(env->key);
		i++;
		env = env->next;
	}
	return (tab);
}

void	exevc(t_cmd *cmd)
{
	char	**paths;
	char	*cmd_path;

	paths = get_paths();
	cmd_path = cmd_file(paths, cmd->args[0]);
	if (!is_regular_file(cmd_path) && ft_strchr(cmd_path, '/'))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		free(cmd_path);
		g_data.exit_status = 126;
		exit(g_data.exit_status);
	}
	else if (!is_regular_file(cmd_path))
		exit(g_data.exit_status);
	free_split(paths);
	execve(cmd_path, cmd->args, envp_totab());
	free(cmd_path);
	perror("bash");
	exit(g_data.exit_status);
}

void	exec_cmd(t_ast_node *ast_elem)
{
	signal(SIGINT, SIG_DFL);
	if (!rediring(ast_elem->content->cmd->redir->head,
			ast_elem->content->cmd))
		exit(g_data.exit_status);
	close_ast_pipe(g_data.ast.root, ast_elem->content->cmd->fd.in,
		ast_elem->content->cmd->fd.out);
	if (dup2(ast_elem->content->cmd->fd.in, STDIN_FILENO) == -1)
		perror("dup2: stdin");
	if (dup2(ast_elem->content->cmd->fd.out, STDOUT_FILENO) == -1)
		perror("dup2: stdout");
	g_data.exit_status = 0;
	if (is_builts(ast_elem->content->cmd))
		builts(ast_elem->content->cmd);
	else if (ast_elem->content->cmd->args[0])
		exevc(ast_elem->content->cmd);
	exit(g_data.exit_status);
}
