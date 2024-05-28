/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:37:27 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/05/20 09:31:25 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <fcntl.h>
#include <vector>
#include "Config.hpp"

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 2048
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
	public:
		TCPServer();
		TCPServer(Config &configs);
		~TCPServer();
		bool	initSocket();
		void	runServer();
		bool	acceptConnection(int serverSD);
		int		readRoutine(int sock);
		void	sendRoutine(int sock);
		void	chunkRequest(std::string request, int *headerStatus);

		void	fillVectorConfigs();
		int		existSocket(int sock);
	//	void	setConfigs(Config &configs)

		std::string& getHeader() const;
		std::string& getBody() const;
		
};



#endif
