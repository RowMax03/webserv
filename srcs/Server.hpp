/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:56:15 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/21 16:43:45 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "sockets/Socket.hpp"
#include <poll.h>
#include <vector>


#define MAX_BUFFER 1024

struct pollfd;

class Server
{
private:
	ServerSocket server;
	std::vector<ClientSocket*> _clients;
	std::vector<pollfd> _pollfds;
	void removeClient(size_t i);
	void addClient(ClientSocket *client);
public:
	Server();
	~Server();
	int Start();
};

// * Will be changed to read from Config later *
Server::Server() : server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8081)
{
	this->_pollfds.push_back((pollfd){server.getFD(), POLLIN, 0});
}

Server::~Server()
{
	for (size_t i = 0; i < _clients.size(); i++) {
		delete _clients[i];
	}
}

void Server::addClient(ClientSocket *client)
{
	_clients.push_back(client);
	_pollfds.push_back((pollfd){client->getFD(), POLLIN, 0});
}

void Server::removeClient(size_t i)
{
	delete _clients[i];
	_clients.erase(_clients.begin() + i - 1);
	_pollfds.erase(_pollfds.begin() + i);
}

int Server::Start()
{
	server.listen_socket(5);
	while (1) {
		if (poll(_pollfds.data(), _pollfds.size(), 100000) < 0) {
			perror("poll failed");
			throw std::runtime_error("poll failed");
		}
		for (size_t i = 0; i < _pollfds.size(); i++) {
			if (_pollfds[i].revents == 0) {
				//std::cout << "Polling i: " << i << std::endl;
				continue;
			}
			if (!i && _pollfds[i].revents & POLLIN) {
				addClient(server.accept_socket());
			}
			else if (_pollfds[i].revents == POLLIN) {
				char buffer[MAX_BUFFER] = {0};
				ssize_t n = _clients[i - 1]->read_socket(buffer ,MAX_BUFFER);
				if (n < 0) {
					perror("read failed");
					removeClient(i);
					continue;
				}
				printf("Received: %s\n", buffer);
				_pollfds[i].events = POLLOUT;
			}
			else if (_pollfds[i].revents == POLLOUT) {
				char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
				ssize_t n = _clients[i - 1]->write_socket(response, strlen(response));
				if (n < 0) {
					perror("write failed");
					removeClient(i);
					continue;
				}
				printf("Sending: %s\n", response);
				_pollfds[i].events = POLLIN;
			}
		}
	}
	return 0;
}
