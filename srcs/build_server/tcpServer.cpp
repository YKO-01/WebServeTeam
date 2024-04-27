/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/04/27 11:07:49 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "build_server.hpp"
#include <fcntl.h>
bool setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        // Failed to get socket flags
        return false;
    }

    // Set the socket to non-blocking mode
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        // Failed to set socket to non-blocking mode
        return false;
    }

    return true;
}
void	tcpServer()
{
	int serverSD = initSocket();
	fd_set FDs, FDsCopy;
	int fdMax;
	int fdNum;
	char buffer[BUFFER_SIZE];

	int conSocket;
	struct sockaddr_in conClientAdd;
	socklen_t clientAddLength = sizeof(conClientAdd);
	memset(&conClientAdd, 0, sizeof(conClientAdd));
	int i;

	FD_ZERO(&FDs);
	FD_SET(serverSD, &FDs);
	fdMax = serverSD;

	while (1)
	{
		FDsCopy = FDs;	
		fdNum = select(fdMax + 1, &FDsCopy, 0, 0, 0);
		if (fdNum >= 0)
		{
			for(i = 0; i < (fdMax + 1); i++)
			{
				if (i > 0 && FD_ISSET(i, &FDsCopy))
				{
					if (i == serverSD)
					{
						conSocket = accept(serverSD, (struct sockaddr*)&conClientAdd, &clientAddLength);
						if (conSocket < 0)
							continue;
						if (!setNonBlocking(conSocket))
						{
							close(conSocket);
							continue;
						}
						FD_SET(conSocket, &FDs);
						if (fdMax < conSocket)
							fdMax = conSocket;
						std::cout << "client with id : " << conSocket << " is connected" << std::endl;
					}
					else
					{
						bzero(buffer, BUFFER_SIZE);
						int bytesNum;
						bytesNum = 0;
						
						bytesNum = recv(i, buffer, BUFFER_SIZE, 0);
						std::cout << " number of bytes : " << bytesNum  << " id : " << i << std::endl;
						if (bytesNum <= 0)
						{
							FD_CLR(i, &FDs);
							close(i);
							break;
						}
						else if (bytesNum > 0)
						{
							std::cout << buffer << std::endl;
							send(i, buffer, bytesNum, 0);
						}
					}
				}
			}
		}
	}
	for (i = 0; i < fdMax + 1; i++)
		close(i);
	close(serverSD);
}



