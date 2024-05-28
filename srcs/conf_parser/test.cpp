#include "conf_parser.hpp"
#include <iostream>

int main() {
    confParser::confParser parser("./config.template.conf");
    std::cout << "Printing configuration for server: " << std::endl;
    parser.serverConfig->print();
    return (0);
}