/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:39 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/30 19:15:16 by mreidenb         ###   ########.fr       */
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

public:
	void parse(const std::string& raw);
	std::vector<std::string> toCgiEnv() const;
	std::string getMethod() const;
	std::string getUrl() const;
	std::string getVersion() const;
	std::string getBody() const;
	std::map<std::string, std::string> getHeaders() const;
};
