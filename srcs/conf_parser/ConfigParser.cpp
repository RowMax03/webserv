/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:36:32 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/28 16:16:47 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ConfigParser.hpp"

namespace Config {

    Parser::Parser(const std::string &configFile) {
        parseConfigFile(configFile);
    }

    Parser::Parser(const Parser &other)
            : _server( Server(other._server)) {}

    Parser &Parser::operator=(const Parser &other) {
        if (this != &other) {
            // delete _server;
            _server =  Server(other._server);
        }
        return *this;
    }

    Parser::~Parser() {
        // delete _server;
    }


    void Parser::parseConfigFile(const std::string &configFile) {
        std::ifstream file(configFile.data());
        if (!file) {
            std::cerr << "Unable to open file: " << configFile << std::endl;
            throw std::invalid_argument("No config File");
            return;
        }
        readConfigFile(file);

        file.close();
    }

    void Parser::readConfigFile(std::ifstream &file) {
        bool boolVarServer = false;
        bool boolVarLocation = false;
        std::string strLine;
        while (std::getline(file, strLine)) {
            strLine = removeLeadingSpaces(strLine);
            if (strLine.empty() || strLine[0] == '#') {
                continue;
            }
            size_t pos = strLine.find_first_of(";{");
            if (pos != std::string::npos) {
                strLine = strLine.substr(0, pos);
            }
            if (strLine.find("server ") != std::string::npos) {
                boolVarServer = true;
                _server =  Server();
                continue;
            }
            if (strLine.find("location ") != std::string::npos) {
                _location =  Location();
                boolVarLocation = true;
            }

            if (boolVarServer && !boolVarLocation && strLine[0] != '}') {
                _server.setConfigValue(tokenize(strLine, " "));
            }

            if (boolVarServer && boolVarLocation && strLine[0] != '}') {
                _location.setConfigValue(tokenize(strLine, " "));
            }

            if (strLine.find("}") != std::string::npos && boolVarLocation && boolVarServer) {
                boolVarLocation = false;
                _server.locations[_location.path] = _location;
                // delete _location;
                continue;
            }
            if (strLine.find("}") != std::string::npos && boolVarServer) {
                boolVarServer = false;
                servers.push_back(_server);
            }
        }
    }

    std::vector <std::string> Parser::tokenize(const std::string &str, const std::string &delim) {
        std::string reducedStr = str;
        std::vector <std::string> tokens;
        size_t start = 0;
        size_t end = reducedStr.find(delim);
        while (end != std::string::npos) {
            if (end > start) {
                tokens.push_back(reducedStr.substr(start, end - start));
            }
            start = end + delim.length();
            while (start < reducedStr.size() && reducedStr[start] == ' ') {
                ++start;
            }
            end = reducedStr.find(delim, start);
        }
        if (start < reducedStr.size()) {
            tokens.push_back(reducedStr.substr(start, end));
        }
        return tokens;
    }


    std::string Parser::removeLeadingSpaces(const std::string &str) {
        std::string::const_iterator i = str.begin();
        while (i != str.end() && (*i == ' ' || *i == '\t' || *i == '\n' || *i == '\r')) {
            ++i;
        }
        return std::string(i, str.end());
    }


} // confParser