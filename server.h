/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 16:02:23 by lxu               #+#    #+#             */
/*   Updated: 2022/02/24 16:02:24 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft/includes/libft.h"

# include <unistd.h>
# include <signal.h>

void	end_of_message(pid_t g_source_pid);
void	signal_handler(int sig);

#endif
