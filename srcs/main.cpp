/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:22:23 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/29 15:29:12 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "conf_parser/ConfigParser.hpp"

int main()
{
	//relative Path from Project root
    Config::Parser config("tester.conf");
    for (size_t i=0; config.servers.size() > i; i++) {
        config.servers[i].validate();
        std::cout << "\nPrinting configuration for server" << i << ":" << std::endl;
        config.servers[i].print();
    }
    Server server(config);
	std::cout << "Server started" << std::endl;
    server.Start();

	return 0;
}
