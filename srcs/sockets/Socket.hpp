/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:15:15 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/11 17:55:00 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <chrono>

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
	private:
		std::string _response;
		const int _server_index;
	public:
		const std::string &getResponse() const;
		int getServerIndex() const;
		void setResponse(const std::string &response);
		ClientSocket(int fd, int server_index);
		int read_socket(void *buf, size_t len);
		void write_socket(const void *buf, size_t len);
};

class ServerSocket : public ASocket
{
	public:
		ServerSocket(int domain, int type, int protocol, u_long interface, int port);
		void listen_socket(int backlog);
		ClientSocket *accept_socket(const int i);
		struct sockaddr_in getAddress();
	private:
		struct sockaddr_in address;
		void bind_socket();
};

