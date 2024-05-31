/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:33:47 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/31 23:48:12 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

#include "../http/parser/HttpParser.hpp"

class CGI
{
	private:
		char** _env;
		const HttpParser &_request;
		void toCharArr(const std::vector<std::string> &envp);
		void deleteEnv();
	public:
		void run();
		CGI(const HttpParser &request);
		~CGI();
};
