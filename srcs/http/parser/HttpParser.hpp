/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:39 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/28 16:47:42 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <map>

class HttpParser
{
private:
	std::map<std::string, std::string> headers;
	std::string method;
	std::string url;
	std::string version;
	std::string body;

public:
	void parse(const std::string& raw);

	std::string getMethod() const;
	std::string getUrl() const;
	std::string getVersion() const;
	std::string getBody() const;
	std::map<std::string, std::string> getHeaders() const;
};
