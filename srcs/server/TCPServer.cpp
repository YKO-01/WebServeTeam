/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/10 15:26:09 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPServer.hpp"
#include <netdb.h>
#include <string>

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
		struct addrinfo hints, *res;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		getaddrinfo(configs[i].get_host().c_str(), std::to_string(configs[i].get_port()).c_str(), &hints, &res);
		serverSD = socket(res->ai_family, res->ai_socktype, 0);
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
		serverAddress.sin_addr.s_addr = ((sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
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
	clients[conSocket] = Client();
	//isChunked[conSocket] = 0;
	// timeval timeStart;
	// gettimeofday(&timeStart, NULL);	
	// timeKeepAlive[conSocket] = timeStart;
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

// __ Get Config Client ________________________________________________________
// =============================================================================
Config	TCPServer::getConfigClient(int sock)
{
	struct sockaddr_in localAddr;
	socklen_t addLen = sizeof(localAddr);

	getsockname(sock, (struct sockaddr*)&localAddr, &addLen);
	int port = ntohs(localAddr.sin_port);
	long host = ntohl(localAddr.sin_addr.s_addr);
//	std::string host = longToString(ipaddr); 
	std::vector<Config>::iterator it = configs.begin();
	while (it != configs.end())
	{
		if (host == TCPUtils::stringToLong(it->get_host()) && port == it->get_port())
			break;
		it++;
	}
	return (*it);
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
				{
					readRoutine(i, &FDSRead, &FDSWrite);
					if (clients[i].getReadNum() == 0)
					{
						clients[i].getHTTPParser()->setBody(clients[i].getRequest());
						clients[i].getHTTPParser()->setConfig(getConfigClient(i));
					}
				}
				else if (FD_ISSET(i, &FDSWrite) && i != existSocket(i))
				{
					if (clients[i].getReadNum() == 0)
						sendRoutine(i, &FDSWrite, &FDSRead);
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
	if (!clients[sock].getIsChunked())
	{
		memset(buffer, 0, BUFFER_SIZE);
		if ((bytesNum = recv(sock, buffer, BUFFER_SIZE, 0)) == 0)
		{
			clients[sock].setReadNum(bytesNum);
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
		clients[sock].setReadNum(bytesNum);
		clients[sock].setRequest(clients[sock].getRequest().append(buffer, bytesNum));
		size_t pos = clients[sock].getRequest().find("\r\n\r\n");
		if (pos != std::string::npos)
		{
			clients[sock].setHTTPParser(new HTTPParser(clients[sock].getRequest().substr(0, pos)));
			clients[sock].setRestBody(clients[sock].getRequest().substr(pos + 4, clients[sock].getRequest().size()));
			clients[sock].setRequest(clients[sock].getRestBody());
			mapHeaders = clients[sock].getHTTPParser()->getHeaders();
			clients[sock].setIsChunked(mapHeaders.find("transfer-encoding") != mapHeaders.end());
		}	
	}
	if (clients[sock].getIsChunked())
	{
		handleChunkedRequest(sock, FDSWrite);
		return ;
	}
	if (bytesNum < BUFFER_SIZE)
	{
		clients[sock].setReadNum(0);
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
	if (clients[sock].getRestBody().size())
	{
		pairChunked = TCPUtils::parseChunkedBody(clients[sock].getRestBody());
		clients[sock].setRequest(clients[sock].getRequest().append(pairChunked.second, pairChunked.second.size()));
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
			std::cout << "end of chunked" << std::endl;
			recv(sock, buffer, 2, 0);
			FD_CLR(sock, &FDs);
			FD_SET(sock, FDSWrite);
			return ;
		}
	}
	n = recv(sock, buffer, std::min(pairChunked.first, (size_t)BUFFER_SIZE), 0);
	if (pairChunked.first > BUFFER_SIZE)
	{
		clients[sock].setRestBody(buffer);
		return ;
	}
	clients[sock].setRequest(clients[sock].getRequest().append(buffer, n));
	//mapRest.erase(mapRest.find(sock));
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
	
	if (clients[sock].getSendNum() && clients[sock].getSendNum() < str.size())
		str = str.substr(clients[sock].getSendNum(), str.size());
	size_t size = str.size();
	if (size > BUFFER_SIZE)
		size = BUFFER_SIZE;
	if ((bytesSend = send(sock, str.c_str(), size + 1, 0)) < 0)
	{
		std::cout << std::strerror(errno) << std::endl;
		FD_CLR(sock, FDSWrite);
		close(sock);
		return ;
	}
	if (clients[sock].getSendNum())
	   	clients[sock].setSendNum(clients[sock].getSendNum() + bytesSend);
	else
	   	clients[sock].setSendNum(bytesSend);
	if (bytesSend == 0 || bytesSend < BUFFER_SIZE)
	{
		clients[sock].setSendNum(0);
		FD_CLR(sock, FDSWrite);
		delete clients[sock].getHTTPParser();
		if (clients[sock].getHTTPParser()->getConnectionType() == HTTP_KEEPALIVE_OFF)
		{
			clients.erase(sock);
			close(sock);
		}
		else
			FD_SET(sock, &FDs);
	}
	struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}