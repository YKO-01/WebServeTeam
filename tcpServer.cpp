/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/03/17 17:09:02 by ayakoubi         ###   ########.fr       */
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
	// set socket option
	int	option = TRUE; 
	setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));  
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
	socklen_t clientAddLength = sizeof(conClientAdd);
	while (1)
	{
		memset(&conClientAdd, 0, sizeof(conClientAdd));
		int conSocket = accept(serverSD, (struct sockaddr*)&conClientAdd, &clientAddLength);
		if (conSocket < 0)
		{
			std::cout << "failed to accept connection to request" << std::endl;
			continue;
		}
		std::cout << "new client with is connected" << std::endl;
		int pid = fork();
		if (pid < 0)
		{
			close(conSocket);
			continue;
		}
		if (pid == 0)
		{
			close(serverSD);
			char buffer[BUFFER_SIZE];
			while (1)
			{
				// send or recieve data from the client
				memset(buffer, 0, strlen(buffer));
				recv(conSocket, buffer, BUFFER_SIZE, 0);
				if ((buffer[0] == 'q' || buffer[0] == 'Q'))
				{
					close(conSocket);
					exit(1);
				}
				std::cout << buffer << std::endl;
				send(conSocket, buffer, BUFFER_SIZE, 0);
			}
		}
		else
			close(conSocket);
	}
	return (0);
}

