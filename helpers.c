#include "helpers.h"

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
		usleep(5000);
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
