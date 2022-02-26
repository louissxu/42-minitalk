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

pid_t	global_pid = 0;

int	send_char(pid_t target_pid, char c)
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

static void end_of_message(pid_t source_pid)
{
	ft_printf("\n");
	send_char(source_pid, 0xFF);
	global_pid = 0;
}

static void byte_handler(unsigned char byte)
{
	static char	char_buff[7];
	static int	buff_i;
	static int	char_width;
	static int	byte_number;
	static pid_t	source_pid;

	if (byte_number == 0)
	{
		if (byte != 0xFF)
		{
			return;
		}
		source_pid = 0;
		byte_number++;
		return ;
	}
	if (byte_number >= 1 && byte_number <= 4)
	{		
		source_pid = source_pid << 8;
		source_pid = source_pid | byte;
		if (byte_number == 4)
		{
			ft_printf("> Incoming message from PID: %i\n", source_pid);
			global_pid = source_pid;
		}
		byte_number++;
		return ;
	}

	if (byte_number >= 5)
	{
		if (char_width == 0)
		{
			if (byte == 0xFF)
			{
				//end of string
				end_of_message(source_pid);
				byte_number = 0;
				return ;
			}
			else if (byte <= 0x7F)
			{
				char_width = 1;
			}
			else if (byte >= 0xC0 && byte <= 0xDF)
			{
				char_width = 2;
			}
			else if (byte >= 0xE0 && byte <= 0xEF)
			{
				char_width = 3;
			}
			else if (byte >= 0xF0 && byte <= 0xF7)
			{
				char_width = 4;
			}
			else if (byte >= 0xF8 && byte <= 0xFB)
			{
				char_width = 5;
			}
			else if (byte >= 0xFC && byte <= 0xFD)
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
		char_buff[buff_i] = byte;
		buff_i++;
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

static void	signal_handler(int sig)
{
	static unsigned char	byte;
	static int	bit_received_count;

	if (sig == SIGUSR1)
	{
		byte = (byte << 1) | 0x01;
	}
	else if (sig == SIGUSR2)
	{
		byte = (byte << 1) | 0x00;
	}
	bit_received_count++;
	if (global_pid != 0)
	{
		// if (bit_received_count == 1)
		// else if (bit_received_count == 8)
		// 	ft_printf(".\n");
		// else
		// 	ft_printf(".");
		
		kill(global_pid, SIGUSR2);
	}
	
	if (bit_received_count == 8)
	{
		byte_handler(byte);
		// ft_printf("byte received. Byte is:");
		// print_byte_as_binary(byte);
		// ft_printf("\n");
		bit_received_count = 0;
		byte = 0x00;
	}
	return ;
}

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	ft_printf("---- Minitalk Server ----\n");
	ft_printf("> Server is running\n");
	ft_printf("> PID is: %d\n", pid);
	ft_printf("-------------------------\n");
	ft_printf("> Waiting for message...\n");
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	while (1)
	{
		sleep(1);
	}
}