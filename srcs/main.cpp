/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nscheefe <nscheefe@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:22:23 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/29 19:58:12 by nscheefe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "conf_parser/ConfigParser.hpp"
#include <set>
#include <iostream>
#include <exception>

int main(int argc, char* argv[])
{
    std::set<int> usedPorts;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_path>" << std::endl;
        return 1;
    }

	Config::Parser config(argv[1]);


    for (size_t i = 0; i < config.servers.size(); ++i) {
        int port = config.servers[i].listen;
        if (usedPorts.find(port) != usedPorts.end()) {
            std::cerr << "Error: Port " << port << " is used more than once." << std::endl;
            return 1;
        }
        usedPorts.insert(port);
        config.servers[i].validate();
    }

    for (size_t i = 0; i < config.servers.size(); i++) {
        std::cout << "\nPrinting configuration for server " << i << ":" << std::endl;
        config.servers[i].print();
    }

    Server server(config);
    std::cout << "Server started" << std::endl;
    server.Start();

    return 0;
}
