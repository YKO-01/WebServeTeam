/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:37:27 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/04 12:57:24 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <fcntl.h>
#include <vector>
#include <cerrno>
#include <algorithm>
#include "../../incs/webserv.h"
#include "../srcs/config/Config.hpp"
#include "../srcs/request/HTTPParser.hpp"

#define BUFFER_SIZE 1024
#define MAX_CONNECTION 10
#define SERVERPORT  5555

class TCPServer
{
	private:
		int serverSD;
		std::vector<int> serverSockets;
		fd_set FDs;
		int fdMax;
		std::string header;
		std::string body;
		std::vector<Config> configs;
		HTTPParser	*httpParser;
	public:
		TCPServer();
		TCPServer(Config &configs);
		~TCPServer();
		bool	initSocket();
		void	runServer();
		bool	acceptConnection(int serverSD);

		int		readRoutine(int sock);
		void	sendRoutine(int sock);

		void	handleTypeRequest(int sock);
		void	handleChunkedRequest(int sock, std::string& request);
		void	handleSimpleRequest(int sock, std::string& request);

		void	chunkRequest(std::string request, int *headerStatus);

		void	fillVectorConfigs();
		int		existSocket(int sock);
	//	void	setConfigs(Config &configs)

		std::string& getHeader() const;
		std::string& getBody() const;
		
};



#endif
