/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 16:01:03 by lxu               #+#    #+#             */
/*   Updated: 2022/02/24 16:01:05 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "libft/includes/libft.h"
# include <signal.h>

int	send_message(pid_t source_pid, pid_t target_pid, char *str);

#endif
