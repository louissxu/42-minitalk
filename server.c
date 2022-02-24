/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:58:42 by lxu               #+#    #+#             */
/*   Updated: 2022/02/24 15:58:43 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	signal_handler(int sig)
{
	static char	character_being_received;
	static int	bit_received_count;

	if (sig == SIGUSR1)
	{
		character_being_received = (character_being_received << 1) | 1;
	}
	else if (sig == SIGUSR2)
	{
		character_being_received = (character_being_received << 1) | 0;
	}
	bit_received_count++;
	if (bit_received_count == 8)
	{
		ft_printf("%c", character_being_received);
		bit_received_count = 0;
	}
	return ;
}

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	ft_printf("---- Minitalk ----\n");
	ft_printf("Server is running. PID is: %d\n", pid);
	ft_printf("Waiting for message...\n");
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	while (1)
	{
		sleep(1);
	}
}
