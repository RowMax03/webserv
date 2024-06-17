/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:41:37 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/17 19:51:12 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationHandler.hpp"

LocationHandler::LocationHandler(const Config::Location &location) : _location(location) {}

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
			if (error == "file not found")
				return "HTTP/1.1 404 File Not Found\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
			else if (error == "file not executable")
				return "HTTP/1.1 403 Forbidden\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
			else
				return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
		}
	} else {
		// Static
		return "HTTP/1.1 418 I'm a teapot\r\nContent-Type: none\r\nContent-Length: 0\r\n\r\n";
	}
}
