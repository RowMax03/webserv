/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:39 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/30 19:39:00 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <map>
#include <cstdlib>

class HttpParser
{
private:
	std::map<std::string, std::string> _headers;
	std::string _method;
	std::string _url;
	std::string _scriptName;
	std::string _pathInfo;
	std::string _queryString;
	std::string _version;
	std::string _body;
	int _contentLength;
	void parseUrl();
	void parse(const std::string& raw);

public:
	HttpParser(const std::string& request);
	~HttpParser();
	std::vector<std::string> toCgiEnv() const;
	std::string getMethod() const;
	std::string getUrl() const;
	std::string getVersion() const;
	std::string getBody() const;
	std::map<std::string, std::string> getHeaders() const;
	bool isCgi;
};
