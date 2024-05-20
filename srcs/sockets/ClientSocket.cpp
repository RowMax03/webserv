/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:34:41 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/20 18:38:27 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

ClientSocket::ClientSocket(int fd)
{
	this->socket_fd = fd;
}

ssize_t	ClientSocket::read_socket(void *buf, size_t len)
{
	return read(this->socket_fd, buf, len);
}

ssize_t	ClientSocket::write_socket(const void *buf, size_t len)
{
	return write(this->socket_fd, buf, len);
}
