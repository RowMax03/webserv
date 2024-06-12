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
        Server() : listen(0), server_name(""), server_names_hash_bucket_size(0), client_max_body_size("") {
            locations = std::map<std::string, Location>();
        }

        Server(const Server &other) : confBase(other), listen(other.listen), server_name(other.server_name),
                                      server_names_hash_bucket_size(other.server_names_hash_bucket_size),
                                      client_max_body_size(other.client_max_body_size),
                                      error_pages(other.error_pages), locations(other.locations) {}

        Server &operator=(const Server &other) {
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

        void setConfigValue(const std::vector <std::string> &configValues) {
            std::string key = configValues[0];
            if (!configValues[1].empty()) {
                if (key == "server_name") {
                    server_name = configValues[1];
                } else if (key == "client_max_body_size") {
                    client_max_body_size = configValues[1];
                } else if (key == "listen") {
                    listen = atoi(configValues[1].c_str());
                    if (listen == 0 || listen > 56000) //@todo max port nochmal nachkucken
                        throw std::invalid_argument("invalid port specified");
                } else if (key == "server_names_hash_bucket_size") {
                    server_names_hash_bucket_size = atoi(configValues[1].c_str());
                    if (server_names_hash_bucket_size == 0)
                        throw std::invalid_argument("invalid server names hash bucket size");
                } else if (key == "error_page") {
                    int status = atoi(configValues[1].c_str());
                    if (status == 0 || status > 600)//@todo max http code nach kucken
                        throw std::invalid_argument("invalid error code specified for error page");
                    if (!configValues[2].empty()) {
                        std::string url = configValues[2];
                        error_pages[status] = ErrorPage(status, url);
                    } else
                        throw std::invalid_argument("no page for error code specified in error page conf");
                } else {
                    throw std::invalid_argument("Invalid key");
                }
            } else
                throw std::invalid_argument("No Value for a key");
        }

        ~Server() {}

        void print() const {
            std::cout << "Server Config: \n\t server_name=" << server_name << "\n\t listen=" << listen
                      << "\n\t server_names_hash_bucket_size=" << server_names_hash_bucket_size
                      << "\n\t client_max_body_size=" << client_max_body_size << std::endl;

            std::cout << "\n\tError Page Configs:" << std::endl;
            for (std::map<int, ErrorPage>::const_iterator it = error_pages.begin();
                 it != error_pages.end(); ++it) {
                it->second.print();
            }

            std::cout << "\n\tLocation Configs:" << std::endl;
            for (std::map<std::string, Location>::const_iterator it = locations.begin();
                 it != locations.end(); ++it) {
                it->second.print();
            }
        }

        void validate() {
            if (this->listen == 0) {
                throw std::invalid_argument("Invalid config");
            }
            if (this->server_name == "" || this->server_name.empty()) {
                throw std::invalid_argument("Invalid config");
            }
            if (this->server_names_hash_bucket_size == 0) {
                throw std::invalid_argument("Invalid config");
            }
            if (this->client_max_body_size.empty() || this->client_max_body_size == "") {
                throw std::invalid_argument("Invalid config");
            }
        }

        int listen;
        std::string server_name;
        int server_names_hash_bucket_size;
        std::string client_max_body_size;
        std::map<int, ErrorPage> error_pages;
        std::map <std::string, Location> locations;
    };
}