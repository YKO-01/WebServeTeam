/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:33:35 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/05/02 10:31:57 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserve.hpp"
#include "build_server.hpp"

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
	TCPServer server;
	server.fillVectorConfigs();
	server.initSocket();
	server.runServer();
	return (0);	
}
