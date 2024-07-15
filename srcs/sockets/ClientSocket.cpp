/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:34:41 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/15 17:23:11 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

ClientSocket::ClientSocket(int fd, int server_index) : _server_index(server_index)
{
	content_length = 0;
	pending_request = false;
	this->socket_fd = fd;
	setLastRequest();
}

int	ClientSocket::read_socket(void *buf, size_t len)
{
	ssize_t n = recv(this->socket_fd, buf, len, O_NONBLOCK);
	if (n == 0) {
		// Other end closed connection
		throw std::runtime_error("Connection closed by peer");
	} else if (n == -1) {
		throw std::runtime_error("Read error: " + std::string(strerror(errno)));
	}
	return n;
}

void	ClientSocket::write_socket(const void *buf, size_t len)
{
	ssize_t n = write(this->socket_fd, buf, len);
	if (n == -1) {
		// An error occurred
		close(this->socket_fd);
		throw std::runtime_error("Write error: " + std::string(strerror(errno)));
	}
	return ;
}

// Getters and setters
const std::string &ClientSocket::getResponse() const {return _response;}
std::string &ClientSocket::getRequest() {return _request;}
std::chrono::time_point<std::chrono::system_clock> ClientSocket::getLastRequest() {return _last_request;}
int ClientSocket::getServerIndex() const {return _server_index;}
void ClientSocket::setResponse(const std::string &response) {_response = response;}
void ClientSocket::setRequest(const std::string &request) {_request += request;}
void ClientSocket::setLastRequest() {_last_request = std::chrono::system_clock::now();}
void ClientSocket::clearRequest() {_request.clear();}

