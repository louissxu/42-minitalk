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

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	ft_printf("------ Minitalk Server ------\n");
	ft_printf("| Server is running         |\n");
	ft_printf("| Server PID is: %-10d |\n", pid);
	ft_printf("-----------------------------\n");
	ft_printf("> Listening for messages...\n");
	ft_printf("| Source PID | Message -----|\n");
	ft_printf("> ");
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	while (1)
	{
		sleep(1);
	}
}
