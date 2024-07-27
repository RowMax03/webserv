/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:39 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/27 21:45:20 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "../../conf_parser/ConfigParser.hpp"

class HttpParser
{
private:
	std::map<std::string, std::string> _headers;
	std::string _rawRequest;
	std::string _method;
	std::string _url;
	std::string _scriptName;
	std::string _path;
	std::string _pathInfo;
	std::string _queryString;
	std::string _version;
	std::string _body;
	std::istringstream _request;
	const Config::Server *_server;
	Config::Location _location;
	int _contentLength;
	int _contentLengthToRead;
	std::string trim(const std::string& str);
	void parseUrl();
	std::string decodeUrl(const std::string& url);
	bool checkRequestLine();
	void parse();
	void validateHeader();
	void updateRequest(const std::string& request);

public:
	bool recivedHeader;
	bool readingBody;
	HttpParser(const HttpParser &other);
	HttpParser &operator=(const HttpParser &other);
	HttpParser(const Config::Server &server);
	~HttpParser();
	void matchLocation();
	void parseBody();
	int &getContentLengthToRead();
	std::vector<std::string> toCgiEnv() const;
	std::string getMethod() const;
	std::string getUrl() const;
	std::string getVersion() const;
	std::string getBody() const;
	std::string getPath() const;
	std::string getScriptName() const;
	const Config::Location &getLocation() const;
	std::map<std::string, std::string> getHeaders() const;
	void updateRawRequest(const std::string& request);
	bool isCgi;
};
