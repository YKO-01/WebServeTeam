/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/05/04 13:20:54 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "build_server.hpp"


// __ Constructor & Destructor _________________________________________________
// =============================================================================
TCPServer::TCPServer():serverSD(-1), fdMax(-1)
{
	FD_ZERO(&FDs);
}

TCPServer::TCPServer(Config &configs):serverSD(-1), fdMax(-1), config(configs)
{
	FD_ZERO(&FDs);
}

TCPServer::~TCPServer()
{
	close(serverSD);
}

void	TCPServer::fillVectorConfigs()
{
	t_serverConfig config[3];
	config[0].ip = "127.0.0.1";
	config[0].port = 5555;
	config[1].ip = "127.0.0.1";
	config[1].port = 8888;
	config[2].ip = "127.0.0.1";
	config[2].port = 7777;
	configs.push_back(config[0]);
	configs.push_back(config[1]);
	configs.push_back(config[2]);
}	

// __ Init Socket  _____________________________________________________________
// =============================================================================
bool	TCPServer::initSocket()
{
	size_t i = -1;
	while (++i < configs.size())
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
		serverAddress.sin_port = htons(configs[i].port);
		
		if (bind(serverSD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		{
			std::cout << "binding failed" << std::endl;
			return (false);
		}
		// listen to the client connection request
		if (listen(serverSD, 10) < 0)
		{
			std::cout << "failed to listening" << std::endl;
			return (false);
		}
		FD_SET(serverSD, &FDs);
    	fdMax = serverSD;
		serverSockets.push_back(serverSD);
	}
	return (true);
}

// __ Set Non Blocking _________________________________________________________
// =============================================================================
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

// __ Accept Connection ________________________________________________________
// =============================================================================
bool TCPServer::acceptConnection(int serverSD)
{
	struct sockaddr_in conClientAdd;
	int conSocket;
	socklen_t clientAddLength = sizeof(conClientAdd);

	memset(&conClientAdd, 0, sizeof(conClientAdd));
	conSocket = accept(serverSD, (struct sockaddr*)&conClientAdd, &clientAddLength);
	if (conSocket < 0)
		return (false);
	if (!setNonBlocking(conSocket))
	{
		close(conSocket);
		return (false);
	}
	FD_SET(conSocket, &FDs);
	if (fdMax < conSocket)
		fdMax = conSocket;
	std::cout << "client with id : " << conSocket << " is connected" << std::endl;
	return (true);
}	

// __ Read Routine _____________________________________________________________
// =============================================================================
int		TCPServer::readRoutine(int sock, std::string& request)
{
	request.clear();
	char buffer[BUFFER_SIZE];
	int bytesNum;

	bytesNum = 0;
	while (1)
	{
		memset(buffer, 0, BUFFER_SIZE);
		bytesNum = recv(sock, buffer, BUFFER_SIZE, 0);
		if (bytesNum == 0)
		{
			FD_CLR(sock, &FDs);
			close(sock);
			break;
		}
		else if (bytesNum > 0)
		{
			std::cout << "===== buffer ======" << std::endl << buffer << std::endl;
			request.append(buffer);
		}
		else if (bytesNum == -1)
			break;
	}
	return (bytesNum);
}

// __ Send Routine _____________________________________________________________
// =============================================================================
void	TCPServer::sendRoutine(int sock, std::string& request)
{
	chunkRequest(request.length(), request);
	send(sock, request.c_str(), request.length(), 0);
}

// __ Exist Socket _____________________________________________________________
// =============================================================================
int		TCPServer::existSocket(int sock)
{
	size_t i;

	for (i = 0; i < serverSockets.size(); i++)
	{
		if (sock == serverSockets[i])
			return (serverSockets[i]);
	}
	return (0);
}

// __ run server  ______________________________________________________________
// =============================================================================
void	TCPServer::runServer()
{
	fd_set FDsCopy;
	int fdNum;
	int	i, j;
	int bytesNum = 0;
	std::string request;

	while (1)
	{
		FDsCopy = FDs;	
		fdNum = select(fdMax + 1, &FDsCopy, 0, 0, 0);
		if (fdNum >= 0)
		{
			for(i = 0; i < (fdMax + 1); i++)
			{
				if (i > 0 && FD_ISSET(i, &FDsCopy) && (j = existSocket(i)))
				{
					if (!acceptConnection(j))
						continue;
				}
				else if (FD_ISSET(i, &FDsCopy))
				{
					//request.clear();
					bytesNum = readRoutine(i, request);
				//	std::cout << request << std::endl;
					if (bytesNum == -1)
						sendRoutine(i, request);
				}
			}
		}
	}
	for (i = 0; i < fdMax + 1; i++)
		close(i);
	for (i = 0; i < serverSockets.size(); i++)
		close(serverSockets[i]);
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

