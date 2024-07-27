/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:56:15 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/27 20:18:17 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../sockets/Socket.hpp"
#include <poll.h>
#include <vector>
#include <chrono>
#include "../conf_parser/ConfigParser.hpp"
#include "LocationHandler.hpp"
#include "../http/response/Response.hpp"
#include "../http/response/SessionHandler.hpp"

#define MAX_BUFFER 10240

class LocationHandler;
struct pollfd;

class Server
{
private:
	const Config::Parser* _conf;
	SessionHandler _sessionHandler;
	std::map<std::string ,LocationHandler*> _locations;
	size_t _server_count;
	std::vector<ServerSocket*> _servers;
	std::vector<ClientSocket*> _clients;
	std::vector<pollfd> _pollfds;
	void removeClient(size_t i);
	void timeoutCheck(size_t i);
	bool checkRevents(size_t i);
	void addClient(ClientSocket *client);
	//pollin fuctions
	void pollin(size_t i);
	std::string readHeaders(size_t i);
	std::string readBody(size_t i, int &content_length);
	void pollout(size_t i);
public:
	Server(const Config::Parser &conf);
	~Server();
	int Start();
};

