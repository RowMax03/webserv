
#include "../../include/conf_parser.hpp"

namespace confParser {

    LocationConfig::LocationConfig()
            : path(""),
              methods({""}),
              redirect_status(0),
              redirect_url(""),
              root(""),
              autoindex(false),
              index(""),
              next(NULL)
    {}

    ServerConfig::ServerConfig()
            : listen(0),
              server_name(""),
              server_names_hash_bucket_size(0),
              error_page(std::make_pair(0, "")),
              client_max_body_size(""),
              locations(NULL)
    {}

    confParser::confParser(const std::string& configFile) : serverConfig(new ServerConfig) {
        parseConfigFile(configFile);
    }

    confParser::confParser(const confParser& other) : serverConfig(new ServerConfig(*other.serverConfig)) {}

    confParser::~confParser() {
        delete serverConfig;
    }

    void confParser::parseConfigFile(const std::string& configFile) {
    }

    void confParser::readConfigFile(int fd) {

    }

    void confParser::setServerConfigValue() {

    }

    void confParser::setLocationConfigValue() {
    }

} // confParser