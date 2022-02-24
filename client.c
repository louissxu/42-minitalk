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
		usleep(50);
	}
	target_pid++;
	return (0);
}

static int	send_string(pid_t target_pid, char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		send_char(target_pid, (int)str[i]);
		i++;
	}
	send_char(target_pid, '\n');
	return (0);
}

int	main(int argc, char **argv)
{
	pid_t	target_pid;

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
	ft_printf("Sending string: <%s>\n", argv[2]);
	send_string(target_pid, argv[2]);
	ft_printf("Message sent.\n");
	return (0);
}
