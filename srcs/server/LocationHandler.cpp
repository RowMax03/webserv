/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:41:37 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/17 18:59:07 by mreidenb         ###   ########.fr       */
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
	return "405";
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
				return "404";
			else if (error == "file not executable")
				return "403";
			else
				return "500";
		}
	} else {
		// Static
		return "HTTP/1.1 418 I'm a teapot\r\n\r\n";
	}
}
