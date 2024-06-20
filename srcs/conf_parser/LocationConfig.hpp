// location_config.hpp
#pragma once

#include "ConfigBase.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>

namespace Config {
    class Location : public confBase {
    public:
        Location();

        Location(const Location &other);

        Location &operator=(const Location &other);

        void setConfigValue(const std::vector <std::string> &configValues);

        ~Location();

        void print() const;

        void validate();

        std::string path;
        std::vector <std::string> methods;
        int redirect_status;
        std::string redirect_url;
        std::string root;
        bool autoindex;
        std::string index;
    };
}