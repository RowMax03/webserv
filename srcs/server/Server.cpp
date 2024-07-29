/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:05:41 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/29 15:31:51 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// * Will be changed to read from Config later *

Server::Server(const Config::Parser &conf) : _conf(&conf) , _server_count(conf.servers.size())
{
	_sessionHandler = SessionHandler();
	for (size_t i = 0; i < _server_count; i++) {
		_servers.push_back(new ServerSocket(AF_INET, SOCK_STREAM, 0, INADDR_ANY, conf.servers[i].listen));
		_servers[i]->listen_socket(1024);
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

void Server::timeoutCheck(size_t i)
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = now - _clients[i - _server_count]->getLastRequest();
	if (elapsed_seconds.count() > 10) { //timeout of 10 seconds
		std::cout << "Client timed out at: " << elapsed_seconds.count() << std::endl;
		removeClient(i);
	}
}

bool Server::checkRevents(size_t i)
{
	bool ret = true;
	if (this->_pollfds[i].revents & POLLHUP)
		ret = false;
	if (this->_pollfds[i].revents & POLLERR)
		ret = false;
	if (this->_pollfds[i].revents & POLLNVAL)
		ret = false;
	if (this->_pollfds[i].revents & POLLPRI)
		ret = false;
	if (!ret)
		removeClient(i);
	return ret;
}

int Server::Start()
{
	while (1) {
		// poll with timeout of 10 seconds
		int ret = poll(_pollfds.data(), _pollfds.size(), 10000);
		if (ret < 0)
			throw std::runtime_error("poll failed");
		for (size_t i = 0; i < _pollfds.size(); i++) {
			if (_pollfds[i].revents == 0 || (i > _server_count - 1 && !checkRevents(i))) {
				continue;
			}
			if (i < _server_count && _pollfds[i].revents & POLLIN) {
				// std::cout << "New connection for server i: " << i << std::endl;
				addClient(_servers[i]->accept_socket(i, _conf->servers[i], _sessionHandler, _clients.size()));
			}
			else if (_pollfds[i].revents == POLLIN) { // ClientSocket is ready to read
				// std::cout << "Client ready to read at i: " << i << std::endl;
				pollin(i);
			}
			else if (_pollfds[i].revents == POLLOUT) { // ClientSocket is ready to write
				// std::cout << "Client ready to write at i: " << i << std::endl;
				pollout(i);
			}
			if (i > _server_count - 1)
				timeoutCheck(i);
		}
	}
	return 0;
}

void Server::pollin(size_t i)
{
	ClientSocket* client = _clients[i - _server_count];
	client->setLastRequest();
	// if (!client->pending_request)
		// client->handler = Response(_conf->servers[client->getServerIndex()], _sessionHandler, _clients.size());
	int &content_length = client->handler.Parser.getContentLengthToRead();
	// std::cout << "Content length: " << content_length << std::endl;
	try {
		if (client->handler.Parser.recivedHeader == false) {
			client->pending_request = true;
			client->handler.recive(readHeaders(i));
		}
		else if (client->handler.Parser.readingBody == true)
			client->handler.recive(readBody(i, content_length));
		if (content_length == 0 && client->handler.Parser.recivedHeader == true && client->handler.Parser.readingBody == false)
			client->pending_request = false;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		removeClient(i);
		return;
	}
	if (!client->pending_request || (!client->handler.Parser.readingBody && client->handler.Parser.recivedHeader)) {
		client->setResponse(client->handler.serialize());
		client->handler.Parser.reset();
		_pollfds[i].events = POLLOUT;
		return;
	}
	std::cout << "Request not fully received, conent length left: " << content_length << std::endl;
}

// Function to read headers and return the headers as a string
std::string Server::readHeaders(size_t i) {
	std::string headers;
	std::vector<char> buffer(MAX_BUFFER); // Use std::vector for automatic memory management
	int bytes_read = _clients[i - _server_count]->read_socket(buffer.data(), MAX_BUFFER);
	// std::cout << "Bytes read: " << bytes_read << std::endl;
	if (bytes_read > 0)
		headers.append(buffer.data(), bytes_read); // Append only the bytes that were actually read
	return headers;
}

// Function to read the body of the request
std::string Server::readBody(size_t i, int &content_length) {
	std::string body;
	int buffersize = std::min(content_length, 10000000);
	std::vector<char> buffer(buffersize); // Use std::vector for automatic memory management
	int bytes_read = 0;
	bytes_read = _clients[i - _server_count]->read_socket(buffer.data(), buffersize); // Use .data() to get the underlying array
	body.assign(buffer.begin(), buffer.begin() + bytes_read); // Assign the read bytes to body
	content_length -= bytes_read;
	return body;
}

void Server::pollout(size_t i)
{
	_clients[i - _server_count]->setLastRequest();
	const std::string &response = _clients[i - _server_count]->getResponse();
	const char *raw = response.c_str();
	try {
		_clients[i - _server_count]->write_socket(raw, response.size()); //will be a sender function later
		std::cout << "Sending: " << response << std::endl;
		_pollfds[i].events = POLLIN;
	}
	catch (const std::exception &e){
		removeClient(i);
		std::cerr << e.what() << std::endl;
	}
}
