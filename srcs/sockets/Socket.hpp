/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:15:15 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/21 15:53:17 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

class ASocket
{
	protected:
		int socket_fd;
	public:
		ASocket();
		virtual ~ASocket();
		int getFD();

};

class ClientSocket : public ASocket
{
	public:
		ClientSocket(int fd);
		ssize_t read_socket(void *buf, size_t len);
		ssize_t write_socket(const void *buf, size_t len);
};

class ServerSocket : public ASocket
{
	public:
		ServerSocket(int domain, int type, int protocol, u_long interface, int port);
		void listen_socket(int backlog);
		ClientSocket *accept_socket();
		struct sockaddr_in getAddress();
	private:
		struct sockaddr_in address;
		void bind_socket();
};

