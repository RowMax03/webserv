/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:15:15 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/20 17:00:21 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

class Socket
{
	private:
		int socket_fd;
		struct sockaddr_in address;
		void bind_socket();
	public:
		Socket(int domain, int type, int protocol, u_long interface, int port);

};
