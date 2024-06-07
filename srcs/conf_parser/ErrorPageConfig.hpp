// error_page_conf.hpp
#pragma once

#include "conf_base.hpp"
#include <iostream>
#include <cstdio>
namespace confParser {
        class ErrorPageConf : public confBase {
            public:
                /* Values */
                int status;

                std::string url;
                
                /* Functions */
                ErrorPageConf();

                ErrorPageConf(int status, const std::string& url = "");

                ErrorPageConf(const ErrorPageConf& other);

                ErrorPageConf& operator=(const ErrorPageConf& other);

                ~ErrorPageConf();

                void print();

                void validate();
        };
}