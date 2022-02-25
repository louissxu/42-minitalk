#ifndef HELPERS_H
# define HELPERS_H

# include "libft/includes/libft.h"

# include <signal.h>

int	send_char(pid_t target_pid, char c);
int	send_string(pid_t target_pid, char* str);

#endif