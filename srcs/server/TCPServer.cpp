/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/04 13:23:33 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPServer.hpp"
#include <netdb.h>

// __ Constructor & Destructor _________________________________________________
// =============================================================================
TCPServer::TCPServer():serverSD(-1), fdMax(-1)
{
	FD_ZERO(&FDs);
}

TCPServer::TCPServer(Config &configs):serverSD(-1), fdMax(-1), configs(configs.get_allInfo())
{
	FD_ZERO(&FDs);
}

TCPServer::~TCPServer()
{
	close(serverSD);
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
	//	std::cout << configs[i].get_port() << std::endl;
		serverAddress.sin_port = htons(configs[i].get_port());
		
		if (bind(serverSD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		{
			std::cout << "binding failed" << std::endl;
			return (false);
		}
		// listen to the client connection request
		if (listen(serverSD, SOMAXCONN) < 0)
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

void	TCPServer::handleTypeRequest(int sock)
{
	char buffer[BUFFER_SIZE];
	int bytesNum;
	std::string request;
	size_t pos = 0;

	while (1)
	{
		memset(buffer, 0, BUFFER_SIZE);
		if ((bytesNum = recv(sock, buffer, BUFFER_SIZE - 1, 0)) == 0)
		{
			FD_CLR(sock, &FDs);
			close(sock);
			break;
		}
		if (bytesNum < 0)
			break;
		request += buffer;

		pos = request.find("\r\n\r\n");
		if (pos < request.size())
		{
			//pos += 4;
			header = request.substr(0, pos);
			body = request.substr(pos + 4, request.size());
			std::cout << header << std::endl;
			std::cout << body << std::endl;
			bool isChunked = header.find("chunked") != std::string::npos;
			if (isChunked)
				handleChunkedRequest(sock, request);
			else
				handleSimpleRequest(sock, request);
			break;
		}
	}
	//if (pos >= request.size())
	//	body = request;
}

void	TCPServer::handleChunkedRequest(int sock, std::string& request)
{
	(void) request;
	char buffer[BUFFER_SIZE];
	int n;

	while (1)
	{
		std::string line;
		std::string data;
		while (1)
		{
			n = recv(sock, buffer, 1, 0);
			if (n == 0)
			{
				FD_CLR(sock, &FDs);
				close(sock);
				return ;
			}
			if (n < 0)
				break;
			line += buffer[0];
			if (line.size() >= 2 && line.find("\r\n") != std::string::npos)
			{
				if (line.size() > 2 && !(std::strtol(line.c_str(), NULL, 16)) && line[0] != '0')
				{
					data += line.substr(0, line.size() - 2);
					line.clear();
				}
				else
				{
					line = line.substr(0, line.size() - 2);
					break;
				}
			}
		}
		long chunkedSize = std::strtol(line.c_str(), NULL, 16);
		if (chunkedSize == 0)
		{
			recv(sock, buffer, 2, 0);
			break;
		}
		while (data.size() < (size_t)chunkedSize)
		{
			memset(buffer, 0, BUFFER_SIZE);
			n = recv(sock, buffer, std::min(data.size() - chunkedSize, (size_t)BUFFER_SIZE - 1), 0);
			if (n < 0)
				break;
			buffer[n] = 0;
			data.append(buffer);
		}
		body += data;
	}
}

void	TCPServer::handleSimpleRequest(int sock, std::string& request)
{
	(void) request;
	char	buffer[BUFFER_SIZE];
	int n;
	std::cout << "here" << std::endl;

	while (1)
	{
		memset(buffer, 0, BUFFER_SIZE);
		n = recv(sock, buffer, BUFFER_SIZE - 1, 0);
		std::cout << "buffer = > " << buffer << std::endl;
		std::cout << n << std::endl;
		if (n < 0)
			break;
		else if (n == 0)
		{
			std::cout << "failed " << std::strerror(errno) << std::endl;
			FD_CLR(sock, &FDs);
			close(sock);
			break;
		}
		else
			body.append(buffer);
	}
}


// __ Read Routine _____________________________________________________________
// =============================================================================
int		TCPServer::readRoutine(int sock)
{
	//char buffer[BUFFER_SIZE];
	//int bytesNum;
	//int headerStatus = 0;
	//std::string request;

	//bytesNum = 0;
	handleTypeRequest(sock);
	std::cout << body << std::endl;
	std::ofstream file("file");
	file << body;
	/*while (1)
	{
		memset(buffer, 0, BUFFER_SIZE);
		bytesNum = recv(sock, buffer, BUFFER_SIZE - 1, 0);
		if (bytesNum < 0)
		{
			FD_CLR(sock, &FDs);
			close(sock);
			break;
		}
		else if (bytesNum == 0)
			break;
		else if (bytesNum > 0)
		{
			buffer[bytesNum] = 0;
			request = buffer;
			chunkRequest(request, &headerStatus);
			if (headerStatus == 1)
			{
				httpParser = new HTTPParser(header);
			}
		}
		if (bytesNum < BUFFER_SIZE - 1)
			break;
	}*/
	return (10);
}

// __ Send Routine _____________________________________________________________
// =============================================================================
void	TCPServer::sendRoutine(int sock)
{
	//chunkRequest(request);
	//body = "HTTP/1.1 200 OK\r\n\r\n<html>\n\r<body>\n\r\rhello\n\r</body>\n</html>";
	std::string response = "HTTP/1.1 200 OK\r\nSet-Cookie: name=ahmed\r\nContent-Type: text/html\r\n\r\n";
    response += "<html>\n";
    response += "<body>\n";
    response += "<h2>Simple Form Example</h2>\n";
    response += "<form action=\"/submit\" method=\"post\">\n";
    response += "  <label for=\"name\">Name:</label><br>\n";
    response += "  <input type=\"text\" id=\"name\" name=\"name\"><br>\n";
    response += "  <label for=\"email\">Email:</label><br>\n";
    response += "  <input type=\"text\" id=\"email\" name=\"email\"><br><br>\n";
    response += "  <input type=\"submit\" value=\"Submit\">\n";
    response += "</form>\n";
    response += "</body>\n";
    response += "</html>\n";
	send(sock, response.c_str(), response.length(), 0);
	header.clear();
	body.clear();
	FD_CLR(sock, &FDs);
	close(sock);
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
					bytesNum = 0;
					bytesNum = readRoutine(i);
					if (bytesNum > 0)
					{
						sendRoutine(i);
					}
				}
			}
		}
	}
	for (i = 0; i < static_cast<int>(serverSockets.size()); i++)
		close(serverSockets[i]);
}

// __ Chunked Request __________________________________________________________
// =============================================================================
void		TCPServer::chunkRequest(std::string request, int *headerStatus)
{
	size_t pos;
	pos = 0;

	std::string dil = "\n\r\n";
	if (*headerStatus == 0)
	{
		if (!header.empty() && header[header.size() - 1] == '\n' && request[0] == '\r' && request[1] == '\n')
		{
			pos = 2;
			//header += request.substr(0, pos);
			*headerStatus = 1;
		}
		else if (!header.empty() && header[header.size() - 2] == '\n' && request[0] == '\n')
		{
			pos = 1;
			//header += request.substr(0, pos);
			*headerStatus = 1;
		}
		else
		{
			pos = request.find(dil);
			if (pos < request.size())
			{
				header.append(request.substr(0, pos));
				*headerStatus = 1;
				pos += 3;
			}
			else
				header.append(request);
		}
	}
	if (*headerStatus == 1)
		body.append(request.substr(pos, request.size()));
	std::cout << "header :" << std::endl << header << std::endl;
	std::cout << "body :" << std::endl << body << std::endl;
}

