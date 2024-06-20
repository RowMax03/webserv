// error_page_conf.hpp
#pragma once

#include "ConfigBase.hpp"
#include <iostream>
#include <cstdio>

namespace Config {
    class ErrorPage : public confBase {
    public:
        /* Values */
        int status;

        std::string url;

        /* Functions */
        ErrorPage();

        ErrorPage(int status, const std::string &url);

        ErrorPage(const ErrorPage &other);

        ErrorPage &operator=(const ErrorPage &other) {
            if (this != &other) {
                status = other.status;
                url = other.url;
            }
            return *this;
        }

        ~ErrorPage();

        void print() const;

        void validate();
    };
}