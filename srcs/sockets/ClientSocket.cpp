/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:34:41 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/21 16:54:23 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

ClientSocket::ClientSocket(int fd)
{
	this->socket_fd = fd;
}

void	ClientSocket::read_socket(void *buf, size_t len)
{
	ssize_t n = read(this->socket_fd, buf, len);
	if (n == 0) {
		// Other end closed connection
		throw std::runtime_error("Connection closed by peer");
	} else if (n == -1) {
		// An error occurred
		throw std::runtime_error("Read error: " + std::string(strerror(errno)));
	}
	return ;
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