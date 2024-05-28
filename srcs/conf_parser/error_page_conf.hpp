// error_page_conf.hpp
#pragma once

#include "conf_base.hpp"
#include <iostream>
#include <cstdio>
namespace confParser {
        class ErrorPageConf : public confBase {
            public:
            ErrorPageConf() : status(501), url("/fallbackError.html") {} // Default constructor
            ErrorPageConf(int status, const std::string& url = "") : status(status), url(url) {
                if (this->url.empty()) {
                    char buffer[50];
                    sprintf(buffer, "default_%d.html", this->status);
                    this->url = buffer;
                }
            }
            ErrorPageConf(const ErrorPageConf& other) : confBase(other), status(other.status), url(other.url) {}
            ErrorPageConf& operator=(const ErrorPageConf& other) { // remove override
                if (this != &other) {
                    status = other.status;
                    url = other.url;
                }
                return *this;
            }
            ~ErrorPageConf() {}

            void print() const {
                std::cout << "ErrorPageConf: status=" << status << ", url=" << url << std::endl;
            }
            int status;
            std::string url;
        };
}