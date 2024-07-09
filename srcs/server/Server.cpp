/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:05:41 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/09 20:46:35 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// * Will be changed to read from Config later *

Server::Server(const Config::Parser &conf) : _conf(&conf) , _server_count(conf.servers.size())
{
	for (size_t i = 0; i < _server_count; i++) {
		_servers.push_back(new ServerSocket(AF_INET, SOCK_STREAM, 0, INADDR_ANY, conf.servers[i].listen));
		_servers[i]->listen_socket(5);
		_pollfds.push_back((pollfd){_servers[i]->getFD(), POLLIN, 0});
		for (std::map<std::string, Config::Location>::const_iterator it = conf.servers[i].locations.begin(); it != conf.servers[i].locations.end(); ++it) {
			_locations[it->first] = (new LocationHandler(it->second, i));
		}
	}
}

Server::~Server()
{
	std::cout << "Server destructor called" << std::endl;
	for (size_t i = 0; i < _clients.size(); i++) {
		delete _clients[i];
	}
	for (size_t i = 0; i < _server_count; i++) {
		delete _servers[i];
	}
	for (std::map<std::string, LocationHandler*>::iterator it = _locations.begin(); it != _locations.end(); ++it) {
		delete it->second;
	}
}

void Server::addClient(ClientSocket *client)
{
	_clients.push_back(client);
	_pollfds.push_back((pollfd){client->getFD(), POLLIN, 0});
}

void Server::removeClient(size_t i)
{
    delete _clients[i - _server_count];
    _clients.erase(_clients.begin() + i - _server_count);
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
				std::cout << "New connection for server i: " << i << std::endl;
				addClient(_servers[i]->accept_socket(i));
			}
			else if (_pollfds[i].revents == POLLIN) { // ClientSocket is ready to read
				std::cout << "Client ready to read at i: " << i << std::endl;
				pollin(i);
			}
			else if (_pollfds[i].revents == POLLOUT) { // ClientSocket is ready to write
				std::cout << "Client ready to write at i: " << i << std::endl;
				pollout(i);
			}
		}
	}
	return 0;
}

void Server::pollin(size_t i)
{
	int bytes_read;
	std::string request;
	bool headers_complete = false;
	try {
		do {
			char buffer[MAX_BUFFER] = {0};
			bytes_read = _clients[i - _server_count]->read_socket(buffer, MAX_BUFFER - 1); // Leave space for null terminator
			request += buffer;
			// Check if the end of the headers section has been reached
			if (request.find("\r\n\r\n") != std::string::npos) {
				headers_complete = true;
				// If handling POST, also check for content length and read until the body is complete
			}
		} while (bytes_read == MAX_BUFFER - 1 && !headers_complete);

		if (!headers_complete) {
			// Optionally, handle the case where the headers are not complete
			// This could involve waiting a bit longer, or logging an incomplete request error
		}

		printf("Received: %s\n", request.c_str());
		// Handler function
		matchLocation(_clients[i - _server_count], request);
		_pollfds[i].events = POLLOUT;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		removeClient(i);
	}
}

void Server::pollout(size_t i)
{
	const char *response = _clients[i - _server_count]->getResponse().c_str();
	try {
		_clients[i - _server_count]->write_socket(response, strlen(response)); //will be a sender function later
		printf("Sending: %s\n", response);
		_pollfds[i].events = POLLIN;
	}
	catch (const std::exception &e){
		removeClient(i);
		std::cerr << e.what() << std::endl;
	}
}



void Server::matchLocation(ClientSocket *client, std::string &raw_request)
{
	try {
		HttpParser request(raw_request);
		std::string path = request.getPath();

		std::string longest_match;
		for (std::map<std::string, LocationHandler*>::const_iterator it = _locations.begin(); it != _locations.end(); ++it) {
			const std::string& location_path = it->first;
			if (it->second->getServerIndex() == client->getServerIndex() &&
				path.compare(0, location_path.size(), location_path) == 0 &&
				location_path.size() > longest_match.size()) {
				longest_match = location_path;
			}
		}
        std::cout << "Location matched: " << longest_match << std::endl;
        Response response(request, _conf->servers[client->getServerIndex()], longest_match, _clients.size());
        response.init();
        client->setResponse(response.serialize());
	}
	catch (const std::exception &e) {
		// something wrong with the request, LocationHandler doesn't throw because it handles errors internally
		//replace with error handler later
		client->setResponse("HTTP/1.1 400 Bad Request\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n");
		std::cerr << e.what() << std::endl;
	}
}
