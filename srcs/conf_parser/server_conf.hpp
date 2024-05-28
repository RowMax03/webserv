// server_conf.hpp
#pragma once

#include "conf_base.hpp"
#include "error_page_conf.hpp"
#include "location_conf.hpp"
#include <map>
#include <iostream>

#include <cstdlib>

namespace confParser {
    class ServerConf : public confBase {
    public:
        ServerConf() : listen(0), server_name(""), server_names_hash_bucket_size(0), client_max_body_size("") {
            // Initialize locations as an empty unordered_map
            locations = std::map<std::string, LocationConfig>();
        }

        ServerConf(const ServerConf& other) : confBase(other), listen(other.listen), server_name(other.server_name), server_names_hash_bucket_size(other.server_names_hash_bucket_size), client_max_body_size(other.client_max_body_size), error_pages(other.error_pages), locations(other.locations) {}        ServerConf& operator=(const ServerConf& other) {
            if (this != &other) {
                listen = other.listen;
                server_name = other.server_name;
                server_names_hash_bucket_size = other.server_names_hash_bucket_size;
                client_max_body_size = other.client_max_body_size;
                locations = other.locations;
                error_pages = other.error_pages;
            }
            return *this;
        }

        void setConfigValue(const std::vector<std::string>& configValues) {
            std::string key = configValues[0];

            if (key == "server_name") {
                server_name = configValues[1];
            } else if (key == "client_max_body_size") {
                client_max_body_size = configValues[1];
            } else if (key == "listen") {
                listen = atoi(configValues[1].c_str());
            } else if (key == "server_names_hash_bucket_size") {
                server_names_hash_bucket_size = atoi(configValues[1].c_str());
            } else if (key == "error_page") {
                ErrorPageConf errorPage;
                errorPage.status = atoi(configValues[1].c_str());
                errorPage.url = configValues[2];
                error_pages[errorPage.status] = errorPage;
            } else {
                throw std::invalid_argument("Invalid key");
            }
        }

        ~ServerConf() {}

        void print() const {
            std::cout << "ServerConf: listen=" << listen << ", server_name=" << server_name
                      << ", server_names_hash_bucket_size=" << server_names_hash_bucket_size
                      << ", client_max_body_size=" << client_max_body_size << std::endl;

            std::cout << "errorpage:" << std::endl;
            for (std::map<int, ErrorPageConf>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it) {
                it->second.print();
            }

            std::cout << "Locations:" << std::endl;
            for (std::map<std::string, LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
                it->second.print();
            }
        }

        int listen;
        std::string server_name;
        int server_names_hash_bucket_size;
        std::string client_max_body_size;
        std::map<int, ErrorPageConf> error_pages;
        std::map<std::string, LocationConfig> locations;
    };
}