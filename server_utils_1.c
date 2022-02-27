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

pid_t	g_source_pid = 0;

static void	byte_handler_1(unsigned char byte, int *byte_number)
{
	static pid_t	source_pid;

	if (*byte_number == 0)
	{
		if (byte != 0x00)
		{
			return ;
		}
		source_pid = 0;
		(*byte_number)++;
		return ;
	}
	if (*byte_number >= 1 && *byte_number <= 4)
	{		
		source_pid = source_pid << 8;
		source_pid = source_pid | byte;
		if (*byte_number == 4)
		{
			ft_printf("%-10i | ", source_pid);
			g_source_pid = source_pid;
		}
		(*byte_number)++;
		return ;
	}
}

static int	calculate_char_width(unsigned char byte)
{
	if (byte <= 0x7F)
		return (1);
	else if (byte >= 0xC0 && byte <= 0xDF)
		return (2);
	else if (byte >= 0xE0 && byte <= 0xEF)
		return (3);
	else if (byte >= 0xF0 && byte <= 0xF7)
		return (4);
	else if (byte >= 0xF8 && byte <= 0xFB)
		return (5);
	else if (byte >= 0xFC && byte <= 0xFD)
		return (6);
	else
		return (-1);
}

static void	byte_handler_2(unsigned char byte, int *char_width)
{
	static char	buff[7];
	static int	buff_i;

	if (*char_width == -1)
	{
		ft_printf("\uFFFD");
		*char_width = 0;
	}
	else
	{
		buff[buff_i] = byte;
		buff_i++;
	}
	if (buff_i == *char_width)
	{
		ft_printf("%s", buff);
		while (buff_i > 0)
		{
			buff_i--;
			buff[buff_i] = '\0';
		}
		*char_width = 0;
	}
}

static void	byte_handler(unsigned char byte)
{
	static int		char_width;
	static int		byte_number;

	if (byte_number >= 0 && byte_number <= 4)
	{
		byte_handler_1(byte, &byte_number);
		return ;
	}
	if (byte_number >= 5 && char_width == 0 && byte == 0xFF)
	{
		end_of_message(g_source_pid);
		byte_number = 0;
		return ;
	}
	if (byte_number >= 5 && char_width == 0)
	{
		char_width = calculate_char_width(byte);
	}
	if (byte_number >= 5)
	{
		byte_handler_2(byte, &char_width);
	}
	return ;
}

void	signal_handler(int sig)
{
	static unsigned char	byte;
	static unsigned char	previous_byte;
	static int				bit_received_count;

	if (previous_byte == 0xFF && sig == SIGUSR1)
		return ;
	if (sig == SIGUSR1)
		byte = (byte << 1) | 0x01;
	else if (sig == SIGUSR2)
		byte = (byte << 1) | 0x00;
	bit_received_count++;
	if (g_source_pid != 0)
		kill(g_source_pid, SIGUSR2);
	if (bit_received_count >= 8)
	{
		previous_byte = byte;
		byte_handler(byte);
		bit_received_count = 0;
		byte = 0x00;
	}
	return ;
}
