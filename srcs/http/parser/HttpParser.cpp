/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:10:46 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/30 19:53:23 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"

HttpParser::HttpParser(const std::string& request) : _contentLength(0), isCgi(false) {
	parse(request);
}

HttpParser::~HttpParser() {}


void HttpParser::parse(const std::string& raw) {
	std::istringstream request(raw);
	std::string line;

	std::getline(request, line);
	std::istringstream firstLine(line);
	firstLine >> _method >> _url >> _version;
	// Split the URL into path and query string
	std::size_t queryPos = _url.find('?');

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
	parseUrl();
}

//envp needs to be null-terminated and freed after use
/**
* std::vector<std::string> vec = {"Hello", "World"};
std::vector<char*> cstrs;
for (size_t i = 0; i < vec.size(); ++i) {
    cstrs.push_back(const_cast<char*>(vec[i].c_str()));
}
char** arr = &cstrs[0];
*/
std::vector<std::string> HttpParser::toCgiEnv() const {
	std::vector<std::string> envp;
	envp.push_back("REQUEST_METHOD=" + _method);
	envp.push_back("SCRIPT_NAME=" + _scriptName);
	envp.push_back("SERVER_PROTOCOL=" + _version);
	std::ostringstream oss;
	oss << _contentLength;
	envp.push_back("CONTENT_LENGTH=" + oss.str());
	envp.push_back("QUERY_STRING=" + _queryString);
	envp.push_back("PATH_INFO=" + _pathInfo);

	if (_method != "GET" && _method != "HEAD") {
		std::map<std::string, std::string>::const_iterator it = _headers.find("Content-Type");
		if (it != _headers.end()) {
			envp.push_back("CONTENT_TYPE=" + it->second);
		}
	}
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
		envp.push_back("HTTP_" + it->first + "=" + it->second);
	}
	return envp;
}

void HttpParser::parseUrl() {
	std::size_t queryPos = _url.find('?');
	std::string path = _url.substr(0, queryPos);
	if (queryPos != std::string::npos) {
		_queryString = _url.substr(queryPos + 1);
		_url = _url.substr(0, queryPos);
	}
	// maybe change this to a regex or config variable later
	std::size_t scriptPos = path.find("/cgi-bin/");
	if (scriptPos != std::string::npos) {
		isCgi = true;
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
