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

    void confParser::readConfigFile(int fd) {
        FILE* file = fdopen(fd, "r");
        if (!file) {
            std::cerr << "Unable to open file descriptor: " << fd << std::endl;
            return;
        }
        char* line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, file)) != -1) {
            std::cout << line << std::endl;
        }
        delete[] line;
        fclose(file);
    }

    }

    void confParser::setServerConfigValue() {

    }

    void confParser::setLocationConfigValue() {
    }

} // confParser