#include "ConfigParser.hpp"
#include <iostream>

int main() {
        Config::Parser config("./config.template.conf");
       /* config.server->validate();
        std::cout << "Printing configuration for server: " << std::endl;
        config.server->print();*/
       for (size_t i=0; config.servers.size() > i; i++) {
           config.servers[i].validate();
           std::cout << "\nPrinting configuration for server" << i << ":" << std::endl;
           config.servers[i].print();
       }
       return (0);
}