/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:33:47 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/05 18:25:38 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <signal.h>


#include "../http/parser/HttpParser.hpp"

class CGI
{
	private:
		char** _env;
		const HttpParser &_request;
		void toCharArr(const std::vector<std::string> &envp);
		void deleteEnv();
	public:
		std::string run();
		CGI(const HttpParser &request);
		~CGI();
};
