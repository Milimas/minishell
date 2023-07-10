/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeihaqi <abeihaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 21:33:56 by abeihaqi          #+#    #+#             */
/*   Updated: 2023/07/08 02:26:50 by abeihaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_quit_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		write(1, "Quit:", 6);
		ft_putnbr_fd(SIGQUIT, 1);
		write(1, "\n", 1);
		signal(SIGQUIT, SIG_DFL);
	}
}

void	sig_ign_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_data.exit_status = 1;
	}
}
