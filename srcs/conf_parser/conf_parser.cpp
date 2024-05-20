
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
        delete serverConfig;
    }

    void confParser::parseConfigFile(const std::string& configFile) {
        FILE* file = fopen(configFile.c_str(), "r");
        if (!file) {
            std::cerr << "Unable to open file: " << configFile << std::endl;
            return;
        }
        readConfigFile(fileno(file));
        fclose(file);
    }

    void confParser::readConfigFile(int fd) {

    }

    void confParser::setServerConfigValue() {

    }

    void confParser::setLocationConfigValue() {
    }

} // confParser