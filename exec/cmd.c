/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouarrak <rouarrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 23:42:59 by rouarrak          #+#    #+#             */
/*   Updated: 2023/06/04 06:40:19 by rouarrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    exec(t_linkedlist *lexer, t_ast ast)
{
    char			*cmd;
	t_cmd			*arg;
	cmd = lexer->head->content;
	arg = ast.root->content->cmd;
	if (!ft_strncmp("exit", cmd, 5))
		bsh_exit(arg);
}
