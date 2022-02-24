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

#include <stdio.h>

void	print_byte_as_binary(char c)
{
	int bit_offset = 0;
	char bit;

	ft_printf("<0b");
	while (bit_offset < 8)
	{
		bit = (c >> (7 - bit_offset)) & 1;
		ft_printf("%d", bit);
		bit_offset++;
	}
	ft_printf(">");
}

static void	signal_handler(int sig)
{
	static unsigned char	char_buff[7];
	static int	buff_i;
	static int	bit_received_count;
	static int	char_width;

	if (sig == SIGUSR1)
	{
		char_buff[buff_i] = (char_buff[buff_i] << 1) | 1;
	}
	else if (sig == SIGUSR2)
	{
		char_buff[buff_i] = (char_buff[buff_i] << 1) | 0;
	}
	bit_received_count++;

	if (bit_received_count == 8)
	{
		if (char_width == 0)
		{
			if (char_buff[buff_i] <= 0x7F)
			{
				char_width = 1;
			}
			else if (char_buff[buff_i] >= 0xC0 && char_buff[buff_i] <= 0xDF)
			{
				char_width = 2;
			}
			else if (char_buff[buff_i] >= 0xE0 && char_buff[buff_i] <= 0xEF)
			{
				char_width = 3;
			}
			else if (char_buff[buff_i] >= 0xF0 && char_buff[buff_i] <= 0xF7)
			{
				char_width = 4;
			}
			else if (char_buff[buff_i] >= 0xF8 && char_buff[buff_i] <= 0xFB)
			{
				char_width = 5;
			}
			else if (char_buff[buff_i] >= 0xFC && char_buff[buff_i] <= 0xFD)
			{
				char_width = 6;
			}
			else
			{
				// Invalid first byte when decoding as UTF-8
				// Change to rplacement character (�)
				char_width = 3;
				char_buff[0] = 0xEF;
				char_buff[1] = 0xBF;
				char_buff[2] = 0xBD;
			}
		}
		//maybe handle the rest of the erroring out if continuing characters are not continuing characters

		buff_i++;
		bit_received_count = 0;
		char_buff[buff_i] = '\0';
		if (buff_i == char_width)
		{
			ft_printf("%s", char_buff);
			while(buff_i > 0)
			{
				buff_i--;
				char_buff[buff_i] = 0;
			}
			char_width = 0;
		}
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