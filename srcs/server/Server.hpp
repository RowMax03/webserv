/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:56:15 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/17 13:53:34 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../sockets/Socket.hpp"
#include <poll.h>
#include <vector>
#include "../conf_parser/ConfigParser.hpp"

#define MAX_BUFFER 1024

struct pollfd;

class Server
{
private:
	Config::Parser _conf;
	std::vector<ServerSocket*> _servers;
	size_t _server_count;
	std::vector<ClientSocket*> _clients;
	std::vector<pollfd> _pollfds;
	void removeClient(size_t i);
	void addClient(ClientSocket *client);
	void pollin(size_t i);
	void pollout(size_t i);
public:
	Server(const Config::Parser &conf);
	~Server();
	int Start();
};
