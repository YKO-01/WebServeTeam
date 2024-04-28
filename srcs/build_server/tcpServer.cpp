/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/04/28 14:55:06 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "build_server.hpp"


// __ Constructor & Destructor _________________________________________________
// =============================================================================
TCPServer::TCPServer():serverSD(-1), fdMax(-1)
{
	FD_ZERO(&FDs);
}

TCPServer::~TCPServer()
{
	close(serverSD);
}


// __ Init Socket  _____________________________________________________________
// =============================================================================
int	TCPServer::initSocket()
{
	// create a socket for the server
	serverSD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSD < 0)
	{
		std::cout << "failed to creation a socket" << std::endl;
		return (false);
	}
	// set socket option
	int	option = TRUE; 	
	setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));  
	// bind this socket to a specific port number
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(SERVERPORT);
	
	if (bind(serverSD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cout << "binding failed" << std::endl;
		return (false);
	}
	// listen to the client connection request
	if (listen(serverSD, 1) < 0)
	{
		std::cout << "failed to listening" << std::endl;
		return (false);
	}
	FD_SET(serverSD, &FDs);
    fdMax = serverSD;
	return (serverSD);
}

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

// __ Run Server  ______________________________________________________________
// =============================================================================
void	TCPServer::runServer()
{
	fd_set FDsCopy;
	int fdNum;
	char buffer[BUFFER_SIZE];
	int	i;
	int conSocket;
	struct sockaddr_in conClientAdd;
	int bytesNum = 0;
	std::string request;

	socklen_t clientAddLength = sizeof(conClientAdd);
	memset(&conClientAdd, 0, sizeof(conClientAdd));
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
						request.clear();
						while (1)
						{
							bzero(buffer, BUFFER_SIZE);
							bytesNum = read(i, buffer, BUFFER_SIZE);
							if (bytesNum == 0)
							{
								FD_CLR(i, &FDs);
								close(i);
								break;
							}
							else if (bytesNum > 0)
								request.append(buffer);
							else if (bytesNum == -1)
								break;
						}
						if (bytesNum == -1)
						{
							chunkRequest(request.length(), request);
							send(i, request.c_str(), request.length(), 0);
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

// __ Chunked Request __________________________________________________________
// =============================================================================
void	TCPServer::chunkRequest(int bytesNum, std::string request)
{
	(void) bytesNum;
	size_t pos;

	std::string dil = "\n\r\n";
	pos = request.find(dil);
	header = request.substr(0, pos);
	std::cout << "pos : " << pos << "   " << request.length() << std::endl; 
	if (header.length() < request.length())
		body = request.substr(pos + 3, request.length());
	else
		body.clear();
	std::cout << "header :" << std::endl << header << std::endl;
	std::cout << "body :" << std::endl << body << std::endl;
}

