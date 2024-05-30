/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:10:46 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/30 18:46:54 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"

void HttpParser::parse(const std::string& raw) {
	std::istringstream request(raw);
	std::string line;

	std::getline(request, line);
	std::istringstream firstLine(line);
	firstLine >> _method >> _url >> _version;

	// Split the URL into path and query string
	std::size_t queryPos = _url.find('?');
	if (queryPos != std::string::npos) {
		_queryString = _url.substr(queryPos + 1);
		_url = _url.substr(0, queryPos);
	}

	while (std::getline(request, line) && line != "\r") {
		std::istringstream headerLine(line);
		std::string key;
		std::getline(headerLine, key, ':');
		std::string value;
		std::getline(headerLine, value);
		_headers[key] = value.substr(1);
	}
	if (_headers.find("Content-Length") != _headers.end()) {
		_contentLength = std::stoi(_headers["Content-Length"]);
		std::vector<char> _bodyChars(_contentLength);
		request.read(&_bodyChars[0], _contentLength);
		_body.assign(_bodyChars.begin(), _bodyChars.end());
	}
	else
		_contentLength = 0;
}

//envp needs to be null-terminated and freed after use
void HttpParser::toCgiEnv(char **envp) const {
	envp[0] = strdup(("REQUEST_METHOD=" + _method).c_str());
	//maybe needs a getter function later depending on implementation
	envp[1] = strdup(("SCRIPT_NAME=" + _scriptName).c_str());
	envp[2] = strdup(("SERVER_PROTOCOL=" + _version).c_str());
	envp[3] = strdup(("CONTENT_LENGTH=" + std::to_string(_contentLength)).c_str());
	envp[4] = strdup(("QUERY_STRING=" + _queryString).c_str());
	//PATH_INFO is the part of the URL after the script name before the query string (if any)
	//Also maybe needs a getter function later depending on implementation
	envp[5] = strdup(("PATH_INFO=" + _pathInfo).c_str());

}

void HttpParser::parseUrl() {
	std::size_t queryPos = _url.find('?');
	std::string path = _url.substr(0, queryPos);
	std::size_t scriptPos = path.find("/cgi-bin/");
	if (scriptPos != std::string::npos) {
		_scriptName = path.substr(scriptPos);
		std::size_t _pathInfoPos = _scriptName.find('/');
		if (_pathInfoPos != std::string::npos) {
			_pathInfo = _scriptName.substr(_pathInfoPos);
			_scriptName = _scriptName.substr(0, _pathInfoPos);
		}
	}
}

std::string HttpParser::getMethod() const { return _method; }
std::string HttpParser::getUrl() const { return _url; }
std::string HttpParser::getVersion() const { return _version; }
std::string HttpParser::getBody() const { return _body; }
std::map<std::string, std::string> HttpParser::getHeaders() const { return _headers; }
