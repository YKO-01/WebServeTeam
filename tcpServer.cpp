/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/03/15 14:54:41 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserve.hpp"

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
	int serverSD;
	// create a socket for the server
	serverSD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSD < 0)
	{
		std::cout << "failed to creation a socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	// bind this socket to a specific port number
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(5555);
	
	if (bind(serverSD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cout << "binding failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	// listen to the client connection request
	if (listen(serverSD, 1) < 0)
	{
		std::cout << "failed to listening" << std::endl;
		exit(EXIT_FAILURE);
	}

	// accept the connection request
	struct sockaddr_in conClientAdd;
	memset(&conClientAdd, 0, sizeof(conClientAdd));
	socklen_t clientAddLength = sizeof(conClientAdd);
	int conSocket = accept(serverSD, (struct sockaddr*)&conClientAdd, &clientAddLength);
	if (conSocket < 0)
	{
		std::cout << "failed to accept connection to request" << std::endl;
		exit(EXIT_FAILURE);
	}
	// send or recieve data from the client
	char recieveData[1024];
	memset(recieveData, 0, strlen(recieveData));
	recv(conSocket, recieveData, 1024, 0);
	std::cout << recieveData << std::endl;	
	// close the socket
	close(serverSD);
	close(conSocket);

	return (0);
}

