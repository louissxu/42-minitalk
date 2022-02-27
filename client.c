/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:58:26 by lxu               #+#    #+#             */
/*   Updated: 2022/02/24 15:58:27 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void	byte_handler(unsigned char byte)
{
	if (byte == 0xFF)
	{
		ft_printf("> Message sent successfully\n");
	}
	exit(0);
}

static void	signal_handler(int sig)
{
	static unsigned char	byte;
	static int				bit_received_count;

	if (sig == SIGUSR1)
	{
		byte = (byte << 1) | 0x01;
		bit_received_count++;
	}
	else if (sig == SIGUSR2)
	{
		byte = 0x00;
		bit_received_count = 0;
	}
	if (bit_received_count == 8)
	{
		byte_handler(byte);
		bit_received_count = 0;
		byte = 0x00;
	}
	return ;
}

static void	write_client_display(pid_t client_pid, pid_t target_pid, char *str)
{
	ft_printf("------ Minitalk Client ------\n");
	ft_printf("| Client PID is: %-10d |\n", client_pid);
	ft_printf("| Target PID is: %-10d |\n", target_pid);
	ft_printf("-----------------------------\n");
	ft_printf("> Sending message...\n");
	ft_printf("| Target PID | Message -----|\n");
	ft_printf("> %-10i | %s\n", target_pid, str);
	ft_printf("> Waiting for server ack...\n");
}

int	main(int argc, char **argv)
{
	pid_t	target_pid;
	pid_t	client_pid;

	if (argc != 3)
	{
		ft_printf("Usage: ./client [pid] [message]\n");
		return (1);
	}
	target_pid = ft_atoi(argv[1]);
	if (target_pid <= 0)
	{
		ft_printf("Invalid PID. Please try again.\n");
		return (1);
	}
	client_pid = getpid();
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	write_client_display(client_pid, target_pid, argv[2]);
	send_message(client_pid, target_pid, argv[2]);
	sleep(1);
	ft_printf("> NO SERVER RESPONSE RECEIVED\n> LIKELY MESSAGE FAILURE\n");
	return (1);
}
