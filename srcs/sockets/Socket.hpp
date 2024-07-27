/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nscheefe <nscheefe@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:15:15 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/27 20:32:56 by nscheefe         ###   ########.fr       */
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

#include "../http/parser/HttpParser.hpp"
#include "../http/response/Response.hpp"

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
		std::chrono::time_point<std::chrono::system_clock> _last_request;
		std::string _response;
		const int _server_index;
	public:
		Response handler;
		bool pending_request;
		const std::string &getResponse() const;
		int getServerIndex() const;
		std::chrono::time_point<std::chrono::system_clock> getLastRequest();
		void setLastRequest();
		void setResponse(const std::string &response);
		void setRequest(const std::string &request);
		void clearRequest();
		ClientSocket(int fd, int server_index);
		ClientSocket(const ClientSocket &other);
		ClientSocket &operator=(const ClientSocket &other);
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

