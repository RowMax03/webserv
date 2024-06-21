/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:41:37 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/21 20:41:30 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationHandler.hpp"

LocationHandler::LocationHandler(const Config::Location &location, const int server_index) : _server_index(server_index) , _location(location) {}

LocationHandler::~LocationHandler() {}

std::string LocationHandler::handleRequest(HttpParser &request)
{
	for (size_t i = 0; i < _location.methods.size(); i++) {
		if (request.getMethod() == _location.methods[i]) {
			return validRequest(request);
		}
	}
	return "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
}

std::string LocationHandler::validRequest(HttpParser &request)
{
	if (request.isCgi) {
		// CGI
		try {
			CGI cgi(request, _location.root);
			return cgi.run();
		}
		catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			std::string error = e.what();
			// Error handling, replace with error handler later
			if (error == "file not found")
				return "HTTP/1.1 404 File Not Found\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
			else if (error == "file not executable")
				return "HTTP/1.1 403 Forbidden\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
			else
				return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
		}
	} else {
		// Static
		std::string file = FileHandler::readFile(_location.root + request.getPath());
		// Set Header
		std::ostringstream oss;
		oss << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: text/html\r\n"
			<< "Content-Length: " << file.length() << "\r\n"
			<< "\r\n"
			<< file;
		return oss.str();
		return "HTTP/1.1 418 I'm a teapot\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
	}
}

int LocationHandler::getServerIndex() const {return _server_index;}
