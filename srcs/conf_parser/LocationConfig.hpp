/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:36:14 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/23 16:19:23 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// location_config.hpp
#pragma once

#include "ConfigBase.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>

namespace Config {
    class Location : public confBase {
    public:
        Location() : path(""), redirect_status(0), client_max_body_size(-1) , redirect_url(""), root(""), autoindex(false), allowCgi(false), index(""), uploadDir(""){}

        Location(const Location &other) : confBase(other), path(other.path),
                                          redirect_status(other.redirect_status), client_max_body_size(other.client_max_body_size),
                                          redirect_url(other.redirect_url), root(other.root),
                                          autoindex(other.autoindex), allowCgi(other.allowCgi), index(other.index) ,
                                          uploadDir(other.uploadDir){
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
                allowCgi = other.allowCgi;
                index = other.index;
                uploadDir = other.uploadDir;
            }
            return *this;
        }
        std::string removeLeadingSlash(const std::string& input) {
            if (!input.empty() && input[0] == '/') {
                return input.substr(1);
            }
            return input;
        }
        std::string removeTrailingSlash(const std::string& input) {
            if (!input.empty() && input[input.length() - 1] == '/') {
                return input.substr(0, input.length() - 1);
            }
            return input;
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
                    root = "/" + removeLeadingSlash(removeTrailingSlash(configValues[1])) + "/";
                } else if (key == "autoindex") {
                    autoindex = (configValues[1] == "on");
                } else if (key == "Cgi"){
                    allowCgi = (configValues[1] == "on");
                } else if (key == "client_max_body_size") {
					client_max_body_size = std::atoi(configValues[1].c_str());
				} else if (key == "index") {
                    index = "/" + removeLeadingSlash(configValues[1]);
                } else if (key == "upload_dir"){
                    uploadDir = "/" + removeLeadingSlash(removeTrailingSlash(configValues[1])) + "/";
                } else if (key == "allow") {
                    for (size_t i = 1; i < configValues.size(); i++) {
                        methods.push_back(configValues[i]);
                    }
                } else{
                    throw std::invalid_argument("Invalid key");
                }
            } else
                throw std::invalid_argument("No Value for a key");
        }

        ~Location() {}
        void print() const {
            std::cout << "\t\tLocation: \n\t\t\tpath=" << path << "\n\t\t\tmethods=" << (methods.size() > 0 ? methods[0] : "")
                      << " " << (methods.size() > 1 ? methods[1] : "") << " " << (methods.size() > 2 ? methods[2] : "") << " " << (methods.size() > 3 ? methods[3] : "") << "\n\t\t\tredirect_status=" << redirect_status
                      << "\n\t\t\tredirect_url=" << redirect_url << "\n\t\t\troot=" << root
                      << "\n\t\t\tautoindex=" << (autoindex ? "on":"off") << "\n\t\t\tindex=" << index << "\n\t\t\tCgi="<< (allowCgi ? "on" : "off" ) << "\n\t\t\tUpload dir="<< uploadDir << std::endl;
        }

        void validate() {
        }

        std::string path;
        std::vector <std::string> methods;
        int redirect_status;
        int client_max_body_size;
        std::string redirect_url;
        std::string root;
        bool autoindex;
        bool allowCgi;
        std::string index;
        std::string uploadDir;
    };
}
