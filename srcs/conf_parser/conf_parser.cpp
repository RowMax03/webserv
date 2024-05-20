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
            size_t pos = strLine.find_first_of(";{");
            if (pos != std::string::npos) {
                strLine = strLine.substr(0, pos);
            }
            if (strLine.find("server ") != std::string::npos) {
                boolVarServer = true;
                continue;
            }
            if (strLine.find("location ") != std::string::npos) {
                boolVarLocation = true;
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

    void confParser::setServerConfigValue(std::vector<std::vector<std::string> >* serverConf) {
        if (serverConf == NULL) {
            std::cerr << "serverConf is null" << std::endl;
            return;
        }
        for (size_t i = 0; i < serverConf->size(); ++i) {
            if ((*serverConf)[i].size() < 2) {
                std::cerr << "Insufficient elements in serverConf at index " << i << std::endl;
                continue;
            }
            std::string key = (*serverConf)[i][0];
            std::string value = (*serverConf)[i][1];
            if (key == "listen") {
                serverConfig->listen = std::atoi(value.data());
            } else if (key == "server_name") {
                serverConfig->server_name = value;
            } else if (key == "server_names_hash_bucket_size") {
                serverConfig->server_names_hash_bucket_size = std::atoi(value.data());
            } else if (key == "client_max_body_size") {
                serverConfig->client_max_body_size = value;
            } else if (key == "error_page" && (*serverConf)[i].size() >= 3) {
                if ((*serverConf)[i].size() < 3) {
                    std::cerr << "Insufficient elements in serverConf for error_page at index " << i << std::endl;
                    continue;
                }
                ErrorPage* newErrorPage = new ErrorPage;
                newErrorPage->status = std::atoi(value.data());
                newErrorPage->url = (*serverConf)[i][2];
                newErrorPage->next = serverConfig->error_pages;
                serverConfig->error_pages = newErrorPage;
            }
        }
    }

    void confParser::setLocationConfigValue(std::vector<std::vector<std::vector<std::string> > >* locationConfs) {
        if (locationConfs == NULL) {
            std::cerr << "locationConfs is null" << std::endl;
            return;
        }
        for (size_t i = 0; i < locationConfs->size(); ++i) {
            LocationConfig* newLocationConfig = new LocationConfig;
            for (size_t j = 0; j < (*locationConfs)[i].size(); ++j) {
                if ((*locationConfs)[i][j].size() < 2) {
                    std::cerr << "Insufficient elements in locationConfs at index " << i << ", " << j << std::endl;
                    continue;
                }
                std::string key = (*locationConfs)[i][j][0];
                std::string value = (*locationConfs)[i][j][1];
                if (key == "location") {
                    newLocationConfig->path = value;
                } else if (key == "allow") {
                    for (size_t k = 1; k < (*locationConfs)[i][j].size(); ++k) {
                        newLocationConfig->methods[k-1] = (*locationConfs)[i][j][k];
                    }
                } else if (key == "return") {
                    newLocationConfig->redirect_status = std::atoi(value.data());
                    newLocationConfig->redirect_url = (*locationConfs)[i][j][2];
                } else if (key == "root") {
                    newLocationConfig->root = value;
                } else if (key == "autoindex") {
                    newLocationConfig->autoindex = (value == "on");
                } else if (key == "index") {
                    newLocationConfig->index = value;
                }
            }
            newLocationConfig->next = serverConfig->locations;
            serverConfig->locations = newLocationConfig;
        }
    }

    std::string confParser::removeLeadingSpaces(const std::string& str) {
        std::string::const_iterator i = str.begin();
        while (i != str.end() && (*i == ' ' || *i == '\t' || *i == '\n' || *i == '\r')) {
            ++i;
        }
        return std::string(i, str.end());
    }

} // confParser