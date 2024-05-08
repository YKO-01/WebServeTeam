/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:33:35 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/05/08 14:04:02 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserve.hpp"
#include "build_server.hpp"
#include "Config.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "bad argument" << std::endl;
		return (0);
	}
	Config configs;
	configs.parssConfigs(av);
	TCPServer server(configs);
	//server.fillVectorConfigs();
	if (!server.initSocket())
		return (1);
	server.runServer();
	return (0);	
}
