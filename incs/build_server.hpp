/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:37:27 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/04/29 11:07:59 by ayakoubi         ###   ########.fr       */
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

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 50
#define MAX_CONNECTION 10
#define SERVERPORT  5555


class TCPServer
{
	private:
		int serverSD;
		fd_set FDs;
		int fdMax;
		std::string header;
		std::string body;
	public:
		TCPServer();
		~TCPServer();
		int		initSocket();
		void	runServer();
		bool	acceptConnection(int serverSD);
		int		readRoutine(int sock, std::string& request);
		void	sendRoutine(int sock, std::string& request);
		void	chunkRequest(int bytesNum, std::string request);

		std::string& getHeader() const;
		std::string& getBody() const;
		
};



#endif