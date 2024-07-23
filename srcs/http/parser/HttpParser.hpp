/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:39 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/23 16:12:39 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <vector>
#include "../../conf_parser/ConfigParser.hpp"

class HttpParser
{
private:
	std::map<std::string, std::string> _headers;
	std::string _method;
	std::string _url;
	std::string _scriptName;
	std::string _path;
	std::string _pathInfo;
	std::string _queryString;
	std::string _version;
	std::string _body;
	const Config::Server *_server;
	int _contentLength;
	std::string trim(const std::string& str);
	void parseUrl();
	std::string decodeUrl(const std::string& url);
	void parse(const std::string& raw);
	bool checkRequestLine();

public:
	HttpParser(const std::string& request, const Config::Server &server);
	~HttpParser();
	std::vector<std::string> toCgiEnv() const;
	std::string getMethod() const;
	std::string getUrl() const;
	std::string getVersion() const;
	std::string getBody() const;
	std::string getPath() const;
	std::string getScriptName() const;
	std::map<std::string, std::string> getHeaders() const;
	bool isCgi;
};
