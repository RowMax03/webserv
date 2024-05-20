/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:27:58 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/20 18:29:34 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/**
 * Constructs a new socket object.
 */
ASocket::ASocket()
{
}

/**
 * Destroys the socket object.
 *
 * This destructor closes the socket file descriptor.
 */
ASocket::~ASocket()
{
	close(this->socket_fd);
}
/**
 * Returns the file descriptor of the socket.
 *
 * @return The file descriptor of the socket.
 */
int ASocket::getFD()
{
	return this->socket_fd;
}
