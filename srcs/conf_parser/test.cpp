#include "../../include/conf_parser.hpp"
#include <iostream>

int main() {
    confParser::confParser parser("./config.template.conf");

    confParser::ServerConfig* serverConfig = parser.serverConfig;
    if (serverConfig == NULL) {
        std::cerr << "serverConfig is null" << std::endl;
        return -1;
    }
    std::cout << "serverconf: " << std::endl;
    std::cout << "Listen: " << serverConfig->listen << std::endl;
    std::cout << "Server name: " << serverConfig->server_name << std::endl;
    std::cout << "Server names hash bucket size: " << serverConfig->server_names_hash_bucket_size << std::endl;
    std::cout << "Client max body size: " << serverConfig->client_max_body_size << std::endl;

    confParser::ErrorPage* errorPage = serverConfig->error_pages;
    while (errorPage != NULL) {
        std::cout << "Error page status: " << errorPage->status << ", URL: " << errorPage->url << std::endl;
        errorPage = errorPage->next;
    }

    confParser::LocationConfig* locationConfig = serverConfig->locations;
    std::cout << "Location conf: " << std::endl;

    while (locationConfig != NULL) {
        std::cout << "--------------------" << std::endl;

        std::cout << "Location path: " << locationConfig->path << std::endl;
        std::cout << "Methods: ";
        for (int i = 0; i < 3; ++i) {
            std::cout << locationConfig->methods[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Redirect status: " << locationConfig->redirect_status << std::endl;
        std::cout << "Redirect URL: " << locationConfig->redirect_url << std::endl;
        std::cout << "Root: " << locationConfig->root << std::endl;
        std::cout << "Autoindex: " << (locationConfig->autoindex ? "on" : "off") << std::endl;
        std::cout << "Index: " << locationConfig->index << std::endl;
        locationConfig = locationConfig->next;
    }

    return 0;
}