/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:37:20 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/21 20:23:34 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "../http/parser/HttpParser.hpp"
#include "../cgi/CGI.hpp"
#include "../http/files/FileHandler.hpp"

class LocationHandler
{
	private:
		const int _server_index;
		const Config::Location &_location;
		std::string validRequest(HttpParser &request);
	public:
		LocationHandler(const Config::Location &location, const int server_index);
		~LocationHandler();
		std::string handleRequest(HttpParser &request);
		int getServerIndex() const;
};
