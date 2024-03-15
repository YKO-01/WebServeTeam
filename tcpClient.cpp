/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcpClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:52:09 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/03/15 14:53:34 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserve.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	int clientSD;
	// create a socket for the client 
	clientSD = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSD < 0)
	{
		std::cout << "failed to creation a socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << clientSD << std::endl;
	// connect the client to a specific server
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(5555);

	if (connect(clientSD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cout << "failed to connect socket server" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "connection exist" << std::endl;
	
	// send data to the server
	send(clientSD, av[1], strlen(av[1]), 0);
	
	// recieve data from the server
	memset(av[1], 0, 1024);
	recv(clientSD, av[1], 1024, 0);
	std::cout << av[1] << std::endl;
	
	return (0);
}
