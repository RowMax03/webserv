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
        ErrorPage() : status(501), url("/fallbackError.html") {}

        ErrorPage(int status, const std::string &url = "") : status(status), url(url) {
		if (this->url.empty()) {
			char buffer[50];
			snprintf(buffer, sizeof(buffer), "default_%d.html", this->status);
			this->url = buffer;
		}
        }

        ErrorPage(const ErrorPage &other) : confBase(other), status(other.status), url(other.url) {}

        ErrorPage &operator=(const ErrorPage &other) {
            if (this != &other) {
                status = other.status;
                url = other.url;
            }
            return *this;
        }

        ~ErrorPage() {}

        void print() const {
            std::cout << "\t\tError Page: \n\t\t\tstatus=" << status << ", \n\t\t\turl=" << url << std::endl;
        }

        void validate() {
        }
    };
}
