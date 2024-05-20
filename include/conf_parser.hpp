#pragma once

#include <string>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
namespace confParser {

    struct ErrorPage {
        int status;
        std::string url;
        ErrorPage* next;
    };

    struct LocationConfig {
        std::string path;
        std::string methods[3]; // GET, POST, DELETE
        int redirect_status;
        std::string redirect_url;
        std::string root;
        bool autoindex;
        std::string index;
        LocationConfig* next;

        LocationConfig();
    };

    struct ServerConfig {
        int listen;
        std::string server_name;
        int server_names_hash_bucket_size;
        ErrorPage* error_pages;
        std::string client_max_body_size;
        LocationConfig* locations;

        ServerConfig();
    };

    class confParser {
    public:
        // Constructor that takes a configuration file
        confParser(const std::string& configFile);
        // Copy constructor
        confParser(const confParser& other);
        // Copy assignment operator
        confParser& operator=(const confParser& other);
        // Destructor
        ~confParser();
        // ServerConfig member variable
        ServerConfig* serverConfig;
    private:
        // Method to parse the configuration file
        void parseConfigFile(const std::string& configFile);
        void readConfigFile(int fd);
        char** tokenize(const char* str, const char* delim, int& numTokens);
        void setServerConfigValue();
        void setLocationConfigValue();
    };

} // confParser