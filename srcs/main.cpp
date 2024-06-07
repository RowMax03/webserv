/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:22:23 by mreidenb          #+#    #+#             */
/*   Updated: 2024/05/21 14:23:13 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "conf_parser/ConfigParser.hpp"

int main()
{
    Config::Parser config("./webserv.conf");
    for (size_t i=0; config.servers.size() > i; i++) {
        config.servers[i].validate();
        std::cout << "\nPrinting configuration for server" << i << ":" << std::endl;
        config.servers[i].print();
    }
    Server server( config.servers[0]);

    server.Start();

	return 0;
}
