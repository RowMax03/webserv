// server_conf.hpp
#pragma once

#include "ConfigBase.hpp"
#include "ErrorPageConfig.hpp"
#include "LocationConfig.hpp"
#include <map>
#include <iostream>

#include <cstdlib>

namespace Config {
    class Server : public confBase {
    public:
        Server();

        Server(const Server &other);

        Server &operator=(const Server &other);

        void setConfigValue(const std::vector <std::string> &configValues);

        ~Server();

        void print() const;

        void validate();

        int listen;
        std::string server_name;
        int server_names_hash_bucket_size;
        std::string client_max_body_size;
        std::map<int, ErrorPage> error_pages;
        std::map <std::string, Location> locations;
    };
}