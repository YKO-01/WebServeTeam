/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:37:27 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/05/09 11:05:25 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILD_SERVER_HPP
#define BUILD_SERVER_HPP

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
#define BUFFER_SIZE 50
#define MAX_CONNECTION 10
#define SERVERPORT  5555

typedef struct serverConfig
{
	std::string ip;
	int port;
}	t_serverConfig;

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
		int		readRoutine(int sock, std::string& request);
		void	sendRoutine(int sock, std::string& request);
		void	chunkRequest(std::string request);

		void	fillVectorConfigs();
		int		existSocket(int sock);
	//	void	setConfigs(Config &configs)

		std::string& getHeader() const;
		std::string& getBody() const;
		
};



#endif