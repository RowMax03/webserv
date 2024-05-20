#pragma once

#include <string>
namespace confParser {

    struct LocationConfig {
        std::string path;
        std::string methods;
        int redirect_status;
        std::string redirect_url;
        std::string root;
        bool autoindex;
        std::string index;
        LocationConfig* next;
    };

    struct ServerConfig {
        int listen;
        std::string server_name;
        int server_names_hash_bucket_size;
        std::pair<int, std::string> error_page;
        std::string client_max_body_size;
        LocationConfig* locations;
    };

    class confParser {
    public:
        // Constructor that takes a configuration file
        confParser(const std::string& configFile);

        // Copy constructor
        confParser(const confParser& other);

        // Destructor
        ~confParser();

        // ServerConfig member variable
        ServerConfig* serverConfig;

    private:
        // Method to parse the configuration file
        void parseConfigFile(const std::string& configFile);
        void readConfigFile(int fd);
        void setServerConfigValue();
        void setLocationConfigValue();
    };

} // confParser