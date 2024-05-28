/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:10:46 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/28 16:37:52 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"

void HttpParser::parse(const std::string& raw) {
	std::istringstream request(raw);
	std::string line;

	std::getline(request, line);
	std::istringstream firstLine(line);
	firstLine >> method >> url >> version;

	while (std::getline(request, line) && line != "\r") {
		std::istringstream headerLine(line);
		std::string key;
		std::getline(headerLine, key, ':');
		std::string value;
		std::getline(headerLine, value);
		headers[key] = value.substr(1);
	}
}

std::string HttpParser::getMethod() const { return method; }
std::string HttpParser::getUrl() const { return url; }
std::string HttpParser::getVersion() const { return version; }
std::map<std::string, std::string> HttpParser::getHeaders() const { return headers; }
