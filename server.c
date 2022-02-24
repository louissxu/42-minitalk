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
	static unsigned int	char_buff;
	static int	bit_received_count;

	if (sig == SIGUSR1)
	{
		char_buff = (char_buff << 1) | 1;
	}
	else if (sig == SIGUSR2)
	{
		char_buff = (char_buff << 1) | 0;
	}
	bit_received_count++;

	if (bit_received_count == 32)
	{
		ft_printf("the int value is: <%32d>\n", char_buff);
		ft_printf("%c", char_buff);
		bit_received_count = 0;
		char_buff = 0;
	}
	// ft_printf("the int at this time is %d\n", char_buff);
	return ;
}

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	ft_printf("---- Minitalk ----\n");
	ft_printf("Server is running. PID is: %d\n", pid);
	ft_printf("Waiting for message...\n");
	// char a[4] = "\u1231";
	// ft_printf("%s", a);
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	while (1)
	{
		sleep(1);
	}
}