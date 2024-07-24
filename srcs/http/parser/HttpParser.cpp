/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:10:46 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/24 16:11:04 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"
#include <iostream>

HttpParser::HttpParser(const std::string& request, const Config::Server &server) :_server(&server), _contentLength(0) , isCgi(false) {
	_request = new std::istringstream(request);
	parse();
}

HttpParser::~HttpParser() {
	delete _request;
}

// parse the raw request into the method, url, version, headers, and body
void HttpParser::parse() {
	std::string line;

	std::getline(*_request, line);
	if (!line.empty() && line.back() == '\r')
		line.pop_back();
	std::istringstream firstLine(line);
	firstLine >> _method >> _url >> _version;
	if (firstLine.fail() || !firstLine.eof() || !checkRequestLine())
		throw std::runtime_error("Invalid request line");
	while (std::getline(*_request, line) && line != "\r") {
		std::istringstream headerLine(line);
		std::string key;
		std::getline(headerLine, key, ':');
		std::string value;
		std::getline(headerLine, value);
		if (headerLine.fail() || key.empty() || value.empty())
			throw std::runtime_error("Invalid header line");
		_headers[key] = trim(value);
	}

	parseUrl();
}

void HttpParser::parseBody() {
	// parse the body of the request
	if (_headers.find("Content-Length") != _headers.end()) {
	_contentLength = std::stoi(_headers["Content-Length"]);
	std::vector<char> _bodyChars(_contentLength);
	_request->read(&_bodyChars[0], _contentLength);
	if (_request->gcount() != _contentLength)
		throw std::runtime_error("Body length does not match Content-Length header body length: " + std::to_string(_request->gcount()) + " vs " + std::to_string(_contentLength));
	_body.assign(_bodyChars.begin(), _bodyChars.end());
	}
}

// change to also throw an exception instead of just returning false later
bool HttpParser::checkRequestLine() {
	if (_method.empty() || _url.empty() || _version.empty())
		return false;
	//else if (_method != "GET" && _method != "DELETE" && _method != "POST")
	//	return false;
	else if (_version != "HTTP/1.0" && _version != "HTTP/1.1")
		return false;
	else if (_url.empty() || _url[0] != '/')
		return false;
	return true;
}

// turn the parsed request into a vector of strings that can be passed to execve later
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
	_url = decodeUrl(_url);
	std::size_t queryPos = _url.find('?');
	_path = _url.substr(0, queryPos);
	if (queryPos != std::string::npos) {
		_queryString = _url.substr(queryPos + 1);
	}
	std::size_t fext = _path.find_last_of('.');
	std::string extension;
	if (fext != std::string::npos) {
		extension = _path.substr(fext + 1);
	}
	for (std::vector<std::string>::const_iterator it = _server->cgi.begin(); it != _server->cgi.end(); ++it) {
		if (*it == extension) {
			std::cout << "CGI extension found: " << extension << std::endl;
			isCgi = true;
			break;
		}
	}
	if (isCgi) {
		_scriptName = _path.substr(_path.find_last_of('/') + 1);
		std::size_t _pathInfoPos = _scriptName.find('/');
		if (_pathInfoPos != std::string::npos) {
			_pathInfo = _scriptName.substr(_pathInfoPos);
			_scriptName = _scriptName.substr(0, _pathInfoPos);
		}
	}
}

std::string HttpParser::decodeUrl(const std::string& url) {
	std::string decoded;
	for (std::size_t i = 0; i < url.size(); ++i) {
		if (url[i] == '%' && i + 2 < url.size()) {
			int value;
			std::istringstream hex(url.substr(i + 1, 2));
			hex >> std::hex >> value;
			if (!hex.fail()) {
				decoded += static_cast<char>(value);
				i += 2;
			}
			else {
				decoded += url[i];
			}
		}
		else {
			decoded += url[i];
		}
	}
	return decoded;
}

std::string HttpParser::getMethod() const { return _method; }
std::string HttpParser::getUrl() const { return _url; }
std::string HttpParser::getVersion() const { return _version; }
std::string HttpParser::getBody() const { return _body; }
std::string HttpParser::getPath() const { return _path; }
std::string HttpParser::getScriptName() const { return _scriptName; }
std::map<std::string, std::string> HttpParser::getHeaders() const { return _headers; }

// trim whitespace from the beginning and end of a string
std::string HttpParser::trim(const std::string& str) {
	std::size_t first = str.find_first_not_of(" \t\r");
	if (first == std::string::npos)
		return "";
	std::size_t last = str.find_last_not_of(" \t\r");
	return str.substr(first, last - first + 1);
}
