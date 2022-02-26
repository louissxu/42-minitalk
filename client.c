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

int	send_char(pid_t target_pid, char c)
{
	char	bit;
	int		bit_offset;

	bit_offset = 0;
	// ft_printf("Sending byte: <0b");
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
		// ft_printf("%i", (int)bit);
		bit_offset++;
		usleep(10000);
		usleep(100);
	}
	// ft_printf(">\n");
	target_pid++;
	return (0);
}

int	send_string(pid_t target_pid, char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		send_char(target_pid, (int)str[i]);
		i++;
	}
	return (0);
}


// Ref: Converting int to bytes https://stackoverflow.com/a/3784478/9160572
static int	send_message(pid_t source_pid, pid_t target_pid, char *str)
{
	send_char(target_pid, 0x00);
	
	send_char(target_pid, (source_pid >> 24) & 0xFF);
	send_char(target_pid, (source_pid >> 16) & 0xFF);
	send_char(target_pid, (source_pid >> 8) & 0xFF);
	send_char(target_pid, source_pid & 0xFF);

	send_string(target_pid, str);
	send_char(target_pid, 0xFF);
	send_char(target_pid, 0xFF);

	return (0);
}

static void byte_handler(unsigned char byte)
{
	if (byte == 0xFF)
	{
		ft_printf("> Message sent successfully!\n");
	}
	exit(0);
}

static void	signal_handler(int sig)
{
	static unsigned char	byte;
	static int	bit_received_count;

	if (sig == SIGUSR1)
	{
		byte = (byte << 1) | 0x01;
		bit_received_count++;
	}
	else if (sig == SIGUSR2)
	{
		// byte = (byte << 1) | 0x00;
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

	ft_printf("------ Minitalk Client ------\n");
	ft_printf("| Client PID is: %-10d |\n", client_pid);
	ft_printf("| Server PID is: %-10d |\n", target_pid);
	ft_printf("-----------------------------\n");
	ft_printf("> Sending message:\n");
	ft_printf("%s\n", argv[2]);

	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);

	ft_printf("> Waiting for acknowledgement from server...\n");
	send_message(client_pid, target_pid, argv[2]);
	
	sleep(1);
	ft_printf("> NO RESPONSE RECEIVED. LIKELY MESSAGE FAILURE\n");
	//ft_printf("Message sent.\n");
	return (1);
}
