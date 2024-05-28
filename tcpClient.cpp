/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcpClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:52:09 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/04/28 10:50:01 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/build_server.hpp"


//void	Write_Routine(int sock, char *buf);
//void	Read_Routine(int sock, char *buf);
void	Write_Routine(int sock, char *buf)
{
	while (1)
	{
		std::cin >> buf;
		send(sock, buf, strlen(buf), 0);
		if (strlen(buf) == 1 && (buf[0] == 'q' || buf[0] == 'Q'))
		{
			close(sock);
			std::cout << "client is disconnected" << std::endl;
			return ;
		}
		memset(buf, 0, strlen(buf));
	}
}

void	Read_Routine(int sock, char *buf)
{
	while (1)
	{
		recv(sock, buf, BUFFER_SIZE, 0);
		if (strlen(buf) <= 0)
		{
			close(sock);
			return ;
		}
		std::cout << "recieve from server : " << buf << std::endl;
		memset(buf, 0, BUFFER_SIZE);
	}
}
int main(int ac, char **av)
{
	(void) av;
	(void) ac;
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
	
	char buffer[BUFFER_SIZE];
//	buffer = NULL;

	int pid = fork();
	if (pid == 0)
	{
	/*	std::cin >> buffer;
		send(clientSD, buffer, BUFFER_SIZE, 0);
		memset(buffer, 0, BUFFER_SIZE);*/
		Write_Routine(clientSD, static_cast<char *>(buffer));
	}
	else
	{
		/*revc(clientSD, (const void*)buffer, BUFFER_SIZE 0);
		if (strlen(buffer) <= 0)
			close(clientSD);
		else
		{
			std::cout << buffer << std::endl;
			memset(buffer, 0, BUFFER_SIZE);
		}*/
		Read_Routine(clientSD, static_cast<char *>(buffer));
	}		
	return (0);
}


