/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:10:46 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/29 18:05:45 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"
#include <iostream>

HttpParser::HttpParser(const Config::Server &server) : _server(&server), _contentLength(0), _contentLengthToRead(0) , recivedHeader(false), readingBody(false), isCgi(false) {
	// std::cout << "Parser Constructor" << std::endl;
}

HttpParser::~HttpParser() {

}

HttpParser::HttpParser(const HttpParser &other) {
	*this = other;
}

HttpParser &HttpParser::operator=(const HttpParser &other) {
	if (this != &other) {
		_method = other._method;
		_url = other._url;
		_scriptName = other._scriptName;
		_path = other._path;
		_pathInfo = other._pathInfo;
		_queryString = other._queryString;
		_version = other._version;
		_body = other._body;
		//_request = other._request;
		_server = other._server;
		_contentLength = other._contentLength;
		_headers = other._headers;
		isCgi = other.isCgi;
		std::string requestContent = other._request.str();
		_request.clear();
		_request.str(requestContent);
	}
	return *this;
}

void HttpParser::updateRawRequest(const std::string& request) {
	_rawRequest += request;
	// std::cout << "Raw Request: " << _rawRequest << std::endl;
	if (!recivedHeader && _rawRequest.find("\r\n\r\n") != std::string::npos) {
		recivedHeader = true;
		updateRequest(_rawRequest);
		parse();
		std::cout << "Parsed request Version: " << _version << std::endl;
	}
	if (recivedHeader && readingBody) {
		if (_contentLengthToRead <= 0) {
			readingBody = false;
			// updateRequest(request);
			parseBody();
		}
	}
}

// parse the raw request into the method, url, version, headers, and body
void HttpParser::parse() {
	// std::cout << "Parsing request" << std::endl;
	std::string line;

	std::getline(_request, line);
	if (!line.empty() && line.back() == '\r')
		line.pop_back();
	std::istringstream firstLine(line);
	firstLine >> _method >> _url >> _version;
	if (firstLine.fail() || !firstLine.eof() || !checkRequestLine())
		throw std::runtime_error("400");
	while (std::getline(_request, line) && line != "\r") {
		std::istringstream headerLine(line);
		std::string key;
		std::getline(headerLine, key, ':');
		std::string value;
		std::getline(headerLine, value);
		if (headerLine.fail() || key.empty() || value.empty())
			throw std::runtime_error("400");
		_headers[key] = trim(value);
	}
	if (_headers.find("Content-Length") != _headers.end())
		_contentLength = std::stoi(_headers["Content-Length"]);
	parseUrl();
	matchLocation();
	validateHeader();
	// std::cout << "Parsed request" << std::endl;
}

// match the location block to the request
void HttpParser::matchLocation() {
	std::string path = _path;
	std::string longest_match;
	if (path.empty())
		path = "/";
	std::map <std::string, Config::Location> locations = _server->locations;
	for (std::map<std::string, Config::Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
		const std::string& location_path = it->first;
		if (path.compare(0, location_path.size(), location_path) == 0 &&
			location_path.size() > longest_match.size()) {
			longest_match = location_path;
		}
	}
	_location = locations[longest_match];
}

void HttpParser::validateHeader() {
	std::cout << "Validating header for Location :" << _location.path << std::endl;
	if (std::find(_location.methods.begin(), _location.methods.end(), _method) == _location.methods.end())
		throw std::runtime_error("405");
	if (_location.client_max_body_size != -1 && _contentLength > _location.client_max_body_size)
		throw std::runtime_error("413");
	if (_method == "")
		throw std::runtime_error("400");
	if (_url == "")
		throw std::runtime_error("400");
	if (_version == "")
		throw std::runtime_error("400");
	if (_method == "POST" && _contentLength > 0)
	{
		readingBody = true;
		size_t endOfHeader = _rawRequest.find("\r\n\r\n");
		_contentLengthToRead = _contentLength - (_rawRequest.length() - (endOfHeader + 4));
	}
}

void HttpParser::updateRequest(const std::string& request) {
	_request.clear();
	_request.str(request);
}

void HttpParser::parseBody() {
	// parse the body of the request
	std::istringstream bodyStream(_rawRequest.substr(_rawRequest.find("\r\n\r\n") + 4));
	if (_body.empty()) {
	std::vector<char> _bodyChars(_contentLength);
	bodyStream.read(&_bodyChars[0], _contentLength);
	if (bodyStream.gcount() != _contentLength)
	{
		std::cout << "Body length does not match Content-Length header body length: " << bodyStream.gcount() << " vs " << _contentLength << std::endl;
		throw std::runtime_error("400");
	}
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
const Config::Location &HttpParser::getLocation() const { return _location; }
int &HttpParser::getContentLengthToRead() { return _contentLengthToRead; }

// trim whitespace from the beginning and end of a string
std::string HttpParser::trim(const std::string& str) {
	std::size_t first = str.find_first_not_of(" \t\r");
	if (first == std::string::npos)
		return "";
	std::size_t last = str.find_last_not_of(" \t\r");
	return str.substr(first, last - first + 1);
}

void HttpParser::reset() {
	_rawRequest.clear();
	_method.clear();
	_url.clear();
	_scriptName.clear();
	_path.clear();
	_pathInfo.clear();
	_queryString.clear();
	_version.clear();
	_body.clear();
	_request.clear();
	_contentLength = 0;
	_contentLengthToRead = 0;
	_headers.clear();
	recivedHeader = false;
	readingBody = false;
	isCgi = false;
}
