// location_config.hpp
#pragma once

#include "ConfigBase.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>

namespace Config {
    class Location : public confBase {
    public:
        Location() : path(""), redirect_status(0), redirect_url(""), root(""), autoindex(false), index("") {}

        Location(const Location &other) : confBase(other), path(other.path),
                                          redirect_status(other.redirect_status),
                                          redirect_url(other.redirect_url), root(other.root),
                                          autoindex(other.autoindex), index(other.index) {
            methods = other.methods;
        }

        Location &operator=(const Location &other) {
            if (this != &other) {
                path = other.path;
                methods = other.methods;
                redirect_status = other.redirect_status;
                redirect_url = other.redirect_url;
                root = other.root;
                autoindex = other.autoindex;
                index = other.index;
            }
            return *this;
        }

        void setConfigValue(const std::vector <std::string> &configValues) {
            std::string key = configValues[0];
            if (!configValues[1].empty()) {
                if (key == "location") {
                    path = configValues[1];
                } else if (key == "return") {
                    if (!configValues[1].empty() && !configValues[2].empty() && atoi(configValues[1].c_str())) {
                        redirect_status = atoi(configValues[1].c_str());
                        if (redirect_status == 0)
                            throw std::invalid_argument("Invalid redirect status");

                        redirect_url = configValues[2];
                    } else
                        throw std::invalid_argument("Invalid redirect values");
                } else if (key == "root") {
                    root = configValues[1];
                } else if (key == "autoindex") {
                    autoindex = (configValues[1] == "on") ? true : ((configValues[1] == "off") ? false
                                                                : throw std::invalid_argument("Invalid autoindex value"));
                } else if (key == "index") {
                    index = configValues[1];
                } else if (key == "allow") {
                    for (size_t i = 1; i < configValues.size(); i++) {
                        methods.push_back(configValues[i]);
                    }
                } else {
                    throw std::invalid_argument("Invalid key");
                }
            } else
                throw std::invalid_argument("No Value for a key");
        }

        ~Location() {}

        void print() const {
            std::cout << "\t\tLocation: \n\t\t\tpath=" << path << "\n\t\t\tmethods=" << (methods.size() > 0 ? methods[0] : "")
                      << " " << (methods.size() > 1 ? methods[1] : "") << "\n\t\t\tredirect_status=" << redirect_status
                      << "\n\t\t\tredirect_url=" << redirect_url << "\n\t\t\troot=" << root
                      << "\n\t\t\tautoindex=" << autoindex << "\n\t\t\tindex=" << index << std::endl;
        }

        void validate() {
        }

        std::string path;
        std::vector <std::string> methods;
        int redirect_status;
        std::string redirect_url;
        std::string root;
        bool autoindex;
        std::string index;
    };
}