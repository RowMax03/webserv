/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:10:46 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/30 18:39:53 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"

void HttpParser::parse(const std::string& raw) {
	std::istringstream request(raw);
	std::string line;

	std::getline(request, line);
	std::istringstream firstLine(line);
	firstLine >> method >> url >> version;

	// Split the URL into path and query string
	std::size_t queryPos = url.find('?');
	if (queryPos != std::string::npos) {
		queryString = url.substr(queryPos + 1);
		url = url.substr(0, queryPos);
	}

	while (std::getline(request, line) && line != "\r") {
		std::istringstream headerLine(line);
		std::string key;
		std::getline(headerLine, key, ':');
		std::string value;
		std::getline(headerLine, value);
		headers[key] = value.substr(1);
	}
	if (headers.find("Content-Length") != headers.end()) {
		contentLength = std::stoi(headers["Content-Length"]);
		std::vector<char> bodyChars(contentLength);
		request.read(&bodyChars[0], contentLength);
		body.assign(bodyChars.begin(), bodyChars.end());
	}
	else
		contentLength = 0;
}

//envp needs to be null-terminated and freed after use
void HttpParser::toCgiEnv(char **envp) const {
	envp[0] = strdup(("REQUEST_METHOD=" + method).c_str());
	//maybe needs a getter function later depending on implementation
	envp[1] = strdup(("SCRIPT_NAME=" + scriptName).c_str());
	envp[2] = strdup(("SERVER_PROTOCOL=" + version).c_str());
	envp[3] = strdup(("CONTENT_LENGTH=" + std::to_string(contentLength)).c_str());
	envp[4] = strdup(("QUERY_STRING=" + queryString).c_str());
	//PATH_INFO is the part of the URL after the script name before the query string (if any)
	//Also maybe needs a getter function later depending on implementation
	envp[5] = strdup(("PATH_INFO=" + pathInfo).c_str());

}

void HttpParser::parseUrl() {
	std::size_t queryPos = url.find('?');
	std::string path = url.substr(0, queryPos);
	std::size_t scriptPos = path.find("/cgi-bin/");
	if (scriptPos != std::string::npos) {
		scriptName = path.substr(scriptPos);
		std::size_t pathInfoPos = scriptName.find('/');
		if (pathInfoPos != std::string::npos) {
			pathInfo = scriptName.substr(pathInfoPos);
			scriptName = scriptName.substr(0, pathInfoPos);
		}
	}
}

std::string HttpParser::getMethod() const { return method; }
std::string HttpParser::getUrl() const { return url; }
std::string HttpParser::getVersion() const { return version; }
std::string HttpParser::getBody() const { return body; }
std::map<std::string, std::string> HttpParser::getHeaders() const { return headers; }
