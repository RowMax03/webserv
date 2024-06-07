// conf_parser.hpp
#pragma once

#include "server_conf.hpp"
#include "location_conf.hpp"
#include <map>
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <ostream>
#include <cstdlib>
#include <sstream>

namespace confParser {
    class confParser {
    public:
        confParser(const std::string &configFile);

        confParser(const confParser &other);

        confParser &operator=(const confParser &other);

        ~confParser();

        ServerConf *serverConfig;

        LocationConfig *locationConfig;

    private:
        std::string removeLeadingSpaces(const std::string &str);

        void parseConfigFile(const std::string &configFile);

        void readConfigFile(std::ifstream& file);

        std::vector <std::string> tokenize(const std::string &str, const std::string &delim);

        void setServerConfigValue(std::vector <std::vector<std::string> > &serverConfVec);

        void setLocationConfigValue(std::vector <std::vector<std::string> > &locationConfVec);
    };
}