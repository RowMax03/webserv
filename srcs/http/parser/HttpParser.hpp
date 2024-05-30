/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:39 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/30 18:30:28 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <map>
#include <cstdlib>

class HttpParser
{
private:
	std::map<std::string, std::string> headers;
	std::string method;
	std::string url;
	std::string scriptName;
	std::string pathInfo;
	std::string queryString;
	std::string version;
	std::string body;
	int contentLength;
	void parseUrl();

public:
	void parse(const std::string& raw);
	void toCgiEnv(char **envp) const;
	std::string getMethod() const;
	std::string getUrl() const;
	std::string getVersion() const;
	std::string getBody() const;
	std::map<std::string, std::string> getHeaders() const;
};
