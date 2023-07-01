/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 23:32:27 by rouarrak          #+#    #+#             */
/*   Updated: 2023/07/01 23:36:10 by rouarrak         ###   ########.fr       */
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

void	check_file(char *cmd)
{
	if (ft_strchr(cmd, '/') && access (cmd, F_OK) == -1)
	{
		ft_putstr_fd("jfd: ", 2);
		perror(cmd);
		g_data.exit_status = 127;
		exit(g_data.exit_status);
	}
	else
	{	
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		g_data.exit_status = 127;
		exit(g_data.exit_status);
	}
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
	check_file(cmd);
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
