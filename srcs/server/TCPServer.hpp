/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:37:27 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/10 14:55:17 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fstream>
#include <fcntl.h>
#include <vector>
#include <cerrno>
#include <algorithm>
#include "../../incs/webserv.h"
#include "../srcs/config/Config.hpp"
#include "../srcs/request/HTTPParser.hpp"
#include "TCPUtils.hpp"
#include "Client.hpp"
#include <cstring>

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
		std::vector<Config> configs;
		std::map<int, Client> clients;
		Map mapHeaders;
	public:
		TCPServer();
		TCPServer(Config &configs);
		~TCPServer();
		bool	initSocket();
		void	runServer();
		bool	acceptConnection(int serverSD, fd_set *FDSRead);

		void	readRoutine(int sock, fd_set *FDSRead, fd_set *FDSWrite);
		void	sendRoutine(int sock, fd_set *FDSWrite, fd_set *FDSRead);

		void	handleTypeRequest(int sock);
		void	handleChunkedRequest(int sock, fd_set *FDSWrite);
		void	handleSimpleRequest(int sock, std::string& request);

		void	chunkRequest(std::string request, int *headerStatus);
		Config	getConfigOfUsingSocket(int sock);

		void	fillVectorConfigs();
		int		existSocket(int sock);
		Config	getConfigClient(int sock);
	//	void	setConfigs(Config &configs)

		std::string& getHeader() const;
		std::string& getBody() const;
		
};



#endif
