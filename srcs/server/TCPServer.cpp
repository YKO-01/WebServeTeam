/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/09 13:17:54 by ayakoubi         ###   ########.fr       */
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
	int count = 0;
	while (++i < configs.size())
	{
		// create a socket for the server
		serverSD = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSD < 0)
		{
			std::cout << "failed to creation a socket" << std::endl;
			continue;
		}
		// set socket option
		int	option = TRUE; 	
		setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));  
		// bind this socket to a specific port number
		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = htonl(TCPUtils::stringToLong(configs[i].get_host()));
		serverAddress.sin_port = htons(configs[i].get_port());
		
		if (bind(serverSD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		{
			std::cout << "binding failed" << std::endl;
			close(serverSD);
			continue ;
		}
		// listen to the client connection request
		if (listen(serverSD, SOMAXCONN) < 0)
		{
			std::cout << "failed to listening" << std::endl;
			close(serverSD);
			continue ;
		}
		FD_SET(serverSD, &FDs);
    	fdMax = serverSD;
		serverSockets.push_back(serverSD);
		count++;
	}
	if (count)
		return (true);
	return (false);
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
bool TCPServer::acceptConnection(int serverSD, fd_set *FDSRead)
{
	(void) FDSRead;
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
	isChunked[conSocket] = 0;
	std::cout << "client with id : " << conSocket << " is connected" << std::endl;
	return (true);
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
	fd_set FDSRead, FDSWrite;
	int fdNum;
	int	i, j;
	
	while (1)
	{
		FDSRead = FDs;
	//	FDSWrite = FDs;
		fdNum = select(fdMax + 1, &FDSRead, &FDSWrite, 0, 0);
		if (fdNum >= 0)
		{
			for(i = 0; i < (fdMax + 1); i++)
			{
				if (FD_ISSET(i, &FDSRead) && (j = existSocket(i)))
				{
					if (!acceptConnection(j, &FDSRead))
						continue;
				}
				else if (FD_ISSET(i, &FDSRead))
					readRoutine(i, &FDSRead, &FDSWrite);
				else if (FD_ISSET(i, &FDSWrite) && i != existSocket(i))
				{
					if (readInfo.find(i) != readInfo.end() && readInfo[i] == 0)
					{
						sendRoutine(i, &FDSWrite, &FDSRead);
						std::ofstream file("file", std::ios::binary);
						file.write(reqInfo[i].c_str(), reqInfo[i].size());
						reqInfo.erase(reqInfo.find(i));
					}

				}
			}
		}
	}
	for (i = 0; i < static_cast<int>(serverSockets.size()); i++)
		close(serverSockets[i]);
}

// __ Read Routine _____________________________________________________________
// =============================================================================
void		TCPServer::readRoutine(int sock, fd_set *FDSRead, fd_set *FDSWrite)
{
	char buffer[BUFFER_SIZE];
	int	bytesNum = 0;

	(void) FDSRead;
	if (!isChunked[sock])
	{
		memset(buffer, 0, BUFFER_SIZE);
		if ((bytesNum = recv(sock, buffer, BUFFER_SIZE, 0)) == 0)
		{
			readInfo[sock] = bytesNum;
			FD_CLR(sock, &FDs);
			FD_SET(sock, FDSWrite);
		}
		if (bytesNum < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				FD_CLR(sock, &FDs);
				close(sock);
			}
		}
		readInfo[sock] = bytesNum;
		reqInfo[sock] = reqInfo[sock].append(buffer, bytesNum);
		size_t pos = reqInfo[sock].find("\r\n\r\n");
		if (pos != std::string::npos)
		{
			httpParser = new HTTPParser(reqInfo[sock].substr(0, pos));
			mapRest[sock] = reqInfo[sock].substr(pos + 4, reqInfo[sock].size());
			mapHeaders = httpParser->getHeaders();
			isChunked[sock] = mapHeaders.find("transfer-encoding") != mapHeaders.end();
		}	
	}
	if (isChunked[sock])
	{
		std::cout << "enter this " << std::endl;
		handleChunkedRequest(sock, FDSWrite);
		return ;
	}
	if (bytesNum < BUFFER_SIZE)
	{
		readInfo[sock] = 0;
		FD_CLR(sock, &FDs);
		FD_SET(sock, FDSWrite);
	}	
}

void	TCPServer::handleChunkedRequest(int sock, fd_set *FDSWrite)
{
	char buffer[BUFFER_SIZE];
	int n;
	std::string line;
	std::string data;
	std::pair<size_t, std::string> pairChunked;

	pairChunked.first = 0;
	if (mapRest[sock].size())
	{
		pairChunked = TCPUtils::parseChunkedBody(mapRest[sock]);
		reqInfo[sock].append(pairChunked.second, pairChunked.second.size());
	}
	if (!pairChunked.first)
	{
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
				line = line.substr(0, line.size() - 2);
				break;
			}
		}
		pairChunked.first = TCPUtils::hexToDecimal(line); 
		if (pairChunked.first == 0)
		{
			recv(sock, buffer, 2, 0);
			FD_CLR(sock, &FDs);
			FD_SET(sock, FDSWrite);
			return ;
		}
	}
	n = recv(sock, buffer, std::min(pairChunked.first, (size_t)BUFFER_SIZE), 0);
	if (pairChunked.first > BUFFER_SIZE)
	{
		mapRest[sock] = buffer;
		return ;
	}
	reqInfo[sock].append(buffer, n);
	mapRest.erase(mapRest.find(sock));
}

// __ Send Routine _____________________________________________________________
// =============================================================================
void	TCPServer::sendRoutine(int sock, fd_set *FDSWrite, fd_set *FDSRead)
{
	(void) FDSRead;
   std::string html_body;
    html_body += "<html>\n";
    html_body += "<body>\n";
    html_body += "<h2>Simple Form Example</h2>\n";
    html_body += "<form action=\"/submit\" method=\"post\">\n";
    html_body += "  <label for=\"name\">Name:</label><br>\n";
    html_body += "  <input type=\"text\" id=\"name\" name=\"name\"><br>\n";
    html_body += "  <label for=\"email\">Email:</label><br>\n";
    html_body += "  <input type=\"text\" id=\"email\" name=\"email\"><br><br>\n";
    html_body += "  <input type=\"submit\" value=\"Submit\">\n";
	html_body += "<input type=\"file\" id=\"avatar\" name=\"avatar\" accept=\"image/png, image/jpeg\" />\n";
	html_body += "</form>\n";
    html_body += "</body>\n";
    html_body += "</html>\n";

	size_t content_length = html_body.size();

	 std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Set-Cookie: name=ahmed\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << content_length << "\r\n";
    response << "\r\n";
	response << html_body;

	std::string str = response.str();
	int bytesSend = 0;
	
	if (writeInfo.find(sock) != writeInfo.end() && writeInfo[sock] < str.size())
		str = str.substr(writeInfo[sock], str.size());
	size_t size = str.size();
	if (size > BUFFER_SIZE)
		size = BUFFER_SIZE;
	if ((bytesSend = send(sock, str.c_str(), size, 0)) < 0)
	{
		std::cout << std::strerror(errno) << std::endl;
		FD_CLR(sock, FDSWrite);
		close(sock);
		return ;
	}
	if (writeInfo.find(sock) != writeInfo.end())
	   	writeInfo[sock] += bytesSend;
	else
	   	writeInfo[sock] = bytesSend;
	if (bytesSend == 0 || bytesSend < BUFFER_SIZE)
	{
		std::cout << "i am here" << std::endl;
		writeInfo.erase(writeInfo.find(sock));
		FD_CLR(sock, FDSWrite);
		//FD_SET(sock, &FDs);
		close(sock);
	}
//	if (httpParser->getConnectionType() == HTTP_KEEPALIVE_OFF)
//		close(sock);
}
