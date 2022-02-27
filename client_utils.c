/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <lxu@student.42adel.org.au>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 02:28:11 by lxu               #+#    #+#             */
/*   Updated: 2022/02/27 02:28:12 by lxu              ###   ########.fr       */
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
		usleep(10000);
		usleep(100);
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
	return (0);
}

// Ref: Converting int to bytes https://stackoverflow.com/a/3784478/9160572
int	send_message(pid_t source_pid, pid_t target_pid, char *str)
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
