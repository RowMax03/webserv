/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:37:20 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/17 18:06:05 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "../http/parser/HttpParser.hpp"
#include "../cgi/CGI.hpp"

class LocationHandler
{
	private:
		const Config::Location &_location;
		std::string validRequest(HttpParser &request);
	public:
		LocationHandler(const Config::Location &location);
		~LocationHandler();
		std::string handleRequest(HttpParser &request);
};
