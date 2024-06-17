/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:56:15 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/17 12:55:52 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "sockets/Socket.hpp"
#include <poll.h>
#include <vector>
#include "conf_parser/ConfigParser.hpp"

#define MAX_BUFFER 1024

struct pollfd;

class Server
{
private:
	Config::Parser _conf;
	std::vector<ServerSocket> _servers;
	size_t _server_count;
	std::vector<ClientSocket*> _clients;
	std::vector<pollfd> _pollfds;
	void removeClient(size_t i);
	void addClient(ClientSocket *client);
	void pollin(size_t i);
	void pollout(size_t i);
public:
	Server(const Config::Parser conf);
	~Server();
	int Start();
};

// * Will be changed to read from Config later *
// is implmented but have to talk of what to use where and how to is in my mind server name the domain but here it is int but a domain is string in my mind
Server::Server(const Config::Parser conf) : _conf(conf) , _server_count(conf.servers.size())
{
	for (size_t i = 0; i < _server_count; i++) {
		_servers.push_back(ServerSocket(AF_INET, SOCK_STREAM, 0, INADDR_ANY, conf.servers[i].listen));
		_servers[i].listen_socket(5);
		_pollfds.push_back((pollfd){_servers[i].getFD(), POLLIN, 0});
	}
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
	while (1) {
		if (poll(_pollfds.data(), _pollfds.size(), 100000) < 0) {
			perror("poll failed");
			throw std::runtime_error("poll failed");
		}
		for (size_t i = 0; i < _pollfds.size(); i++) {
			if (_pollfds[i].revents == 0) {
				continue;
			}
			if (i < _server_count && _pollfds[i].revents & POLLIN) {
				addClient(_servers[i].accept_socket());
			}
			else if (_pollfds[i].revents == POLLIN) { // ClientSocket is ready to read
				pollin(i);
			}
			else if (_pollfds[i].revents == POLLOUT) { // ClientSocket is ready to write
				pollout(i);
			}
		}
	}
	return 0;
}

void Server::pollin(size_t i)
{
	char buffer[MAX_BUFFER] = {0};
	try {
		_clients[i - 1]->read_socket(buffer ,MAX_BUFFER); //will be a handler function later
		printf("Received: %s\n", buffer);
		_pollfds[i].events = POLLOUT;
	}
	catch (const std::exception &e){
		std::cerr << e.what() << std::endl;
		removeClient(i);
	}
}

void Server::pollout(size_t i)
{
	char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	try {
		_clients[i - 1]->write_socket(response, strlen(response)); //will be a sender function later
		printf("Sending: %s\n", response);
		_pollfds[i].events = POLLIN;
	}
	catch (const std::exception &e){
		removeClient(i);
		std::cerr << e.what() << std::endl;
	}
}
