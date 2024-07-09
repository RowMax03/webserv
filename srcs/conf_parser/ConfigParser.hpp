/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nscheefe <nscheefe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:36:25 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/09 23:36:26 by nscheefe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// conf_parser.hpp
#pragma once

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include <map>
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <ostream>
#include <cstdlib>
#include <sstream>

namespace Config {
    class Parser {
    private:

        Server *_server;

        Location *_location;
    public:
        Parser(const std::string &configFile);

        Parser(const Parser &other);

        Parser &operator=(const Parser &other);

        ~Parser();


        std::vector<Server> servers;

    private:
        std::string removeLeadingSpaces(const std::string &str);

        void parseConfigFile(const std::string &configFile);

        void readConfigFile(std::ifstream &file);

        std::vector <std::string> tokenize(const std::string &str, const std::string &delim);

    };
}