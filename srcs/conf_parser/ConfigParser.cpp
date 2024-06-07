#include "conf_parser.hpp"

namespace confParser {

    confParser::confParser(const std::string& configFile)
    {
        parseConfigFile(configFile);
    }

    confParser::confParser(const confParser& other)
            : serverConfig(new ServerConf(*other.serverConfig))
    {}

    confParser& confParser::operator=(const confParser& other) {
        if (this != &other) {
            delete serverConfig;
            serverConfig = new ServerConf(*other.serverConfig);
        }
        return *this;
    }

    confParser::~confParser() {
        delete serverConfig;
    }


    void confParser::parseConfigFile(const std::string& configFile) {
        std::ifstream file(configFile.data());
        if (!file) {
            std::cerr << "Unable to open file: " << configFile << std::endl;
            return;
        }
        readConfigFile(file);

        file.close();
    }

    void confParser::readConfigFile(std::ifstream& file) {
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
                serverConfig = new ServerConf;
                continue;
            }
            if (strLine.find("location ") != std::string::npos) {
                locationConfig = new LocationConfig;
                boolVarLocation = true;
            }

            if (boolVarServer && !boolVarLocation && strLine[0] != '}') {
                serverConfig->setConfigValue(tokenize(strLine, " "));
            }

            if (boolVarServer && boolVarLocation && strLine[0] != '}') {
               locationConfig->setConfigValue(tokenize(strLine, " "));
            }

            if(strLine.find("}") != std::string::npos && boolVarLocation && boolVarServer) {
                boolVarLocation = false;
                serverConfig->locations[locationConfig->path] = *locationConfig;
                delete locationConfig;
                continue;
            }
            if(strLine.find("}") != std::string::npos && boolVarServer) {
                boolVarServer = false;
                break;
            }
        }
    }

    std::vector<std::string> confParser::tokenize(const std::string& str, const std::string& delim) {
        std::string reducedStr = str;
        std::vector<std::string> tokens;
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


    std::string confParser::removeLeadingSpaces(const std::string& str) {
        std::string::const_iterator i = str.begin();
        while (i != str.end() && (*i == ' ' || *i == '\t' || *i == '\n' || *i == '\r')) {
            ++i;
        }
        return std::string(i, str.end());
    }




} // confParser