#include "../../include/conf_parser.hpp"

namespace confParser {

    LocationConfig::LocationConfig()
            : path(""), redirect_status(0), redirect_url(""), root(""),
              autoindex(false), index(""), next(NULL)
    {
        methods[0] = "";
        methods[1] = "";
        methods[2] = "";
    }

    ServerConfig::ServerConfig()
            : listen(0), server_name(""), server_names_hash_bucket_size(0),
              error_pages(NULL), client_max_body_size(""), locations(NULL)
    {}

    confParser::confParser(const std::string& configFile)
            : serverConfig(new ServerConfig)
    {
        parseConfigFile(configFile);
    }

    confParser::confParser(const confParser& other)
            : serverConfig(new ServerConfig(*other.serverConfig))
    {}

    confParser& confParser::operator=(const confParser& other) {
        if (this != &other) {
            delete serverConfig;
            serverConfig = new ServerConfig(*other.serverConfig);
        }
        return *this;
    }

    confParser::~confParser() {
        if (serverConfig) {
            ErrorPage* errorPage = serverConfig->error_pages;
            while (errorPage != NULL) {
                ErrorPage* next = errorPage->next;
                delete errorPage;
                errorPage = next;
            }

            LocationConfig* locationConfig = serverConfig->locations;
            while (locationConfig != NULL) {
                LocationConfig* next = locationConfig->next;
                delete locationConfig;
                locationConfig = next;
            }

            delete serverConfig;
        }
    }

    void confParser::parseConfigFile(const std::string& configFile) {
        std::ifstream file(configFile.data());
        if (!file) {
            std::cerr << "Unable to open file: " << configFile << std::endl;
            return;
        }
        std::vector<std::vector<std::string> >* serverConf = NULL;
        std::vector<std::vector<std::vector<std::string> > >* locationConfs = NULL;
        readConfigFile(file, serverConf, locationConfs);
        setServerConfigValue(serverConf);
        setLocationConfigValue(locationConfs);
        delete serverConf;
        delete locationConfs;
        file.close();
    }

    void confParser::readConfigFile(std::ifstream& file, std::vector<std::vector<std::string> >*& serverConf, std::vector<std::vector<std::vector<std::string> > >*& locationConfs) {
        bool boolVarServer = false;
        bool boolVarLocation = false;
        serverConf = new std::vector<std::vector<std::string> >;
        locationConfs = new std::vector<std::vector<std::vector<std::string> > >;
        std::string strLine;
        while (std::getline(file, strLine)) {
            strLine = removeLeadingSpaces(strLine);
            if (strLine.empty() || strLine[0] == '#') {
                continue;
            }
            size_t pos = strLine.find_first_of(";{}");
            if (pos != std::string::npos) {
                strLine = strLine.substr(0, pos + 1); // +1 to include the character at the found position
            }
            if (strLine.find("server {") != std::string::npos) {
                boolVarServer = true;
                continue;
            }
            if (strLine.find("location") != std::string::npos) {
                boolVarLocation = true;
            }
            pos = strLine.find_first_of(";{");
            if (pos != std::string::npos) {
                strLine = strLine.substr(0, pos);
            }

            if (boolVarServer && !boolVarLocation && strLine[0] != '}') {

                std::vector<std::string> tokens = tokenize(strLine, " ");
                serverConf->push_back(tokens);
            }

            if (boolVarServer && boolVarLocation && strLine[0] != '}') {
                std::vector<std::string> tokens = tokenize(strLine, " ");
                if (strLine.find("location") != std::string::npos) {
                    locationConfs->push_back(std::vector<std::vector<std::string> >());
                }
                locationConfs->back().push_back(tokens);
            }

            if(strLine.find("}") != std::string::npos && boolVarLocation && boolVarServer) {
                boolVarLocation = false;
                continue;
            }
            if(strLine.find("}") != std::string::npos && boolVarServer) {
                boolVarServer = false;
                break;
            }
        }
    }

    std::vector<std::string> confParser::tokenize(const std::string& str, const std::string& delim) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delim);
        while (end != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delim.length();
            end = str.find(delim, start);
        }
        tokens.push_back(str.substr(start, end));
        return tokens;
    }

    void confParser::setServerConfigValue() {

    }

    void confParser::setLocationConfigValue() {
    }

} // confParser