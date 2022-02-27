/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <lxu@student.42adel.org.au>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 14:45:24 by lxu               #+#    #+#             */
/*   Updated: 2022/02/27 14:45:26 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	send_char(pid_t target_pid, char c)
{
	char	bit;
	int		bit_offset;

	bit_offset = 0;
	while (bit_offset < 8)
	{
		bit = (c >> (7 - bit_offset)) & 1;
		if (bit == 1)
		{
			kill(target_pid, SIGUSR1);
		}
		else
		{
			kill(target_pid, SIGUSR2);
		}
		bit_offset++;
		usleep(100);
	}
	target_pid++;
	return (0);
}

void	end_of_message(pid_t g_source_pid)
{
	ft_printf("\n");
	ft_printf("> ");
	send_char(g_source_pid, 0xFF);
	g_source_pid = 0;
}
