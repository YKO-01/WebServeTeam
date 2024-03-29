/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:37:56 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/03/29 17:55:36 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "build_server.hpp"

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
	
	int serverSD = initSocket();
	fd_set	read_fd_set;

	/*initialize all connection and set the first entery to server fd*/
	int all_connection[MAX_CONNECTION];
	int i;
	for (i = 0; i < MAX_CONNECTION; i++)
		all_connection[i] = -1;
	all_connection[0] = serverSD;
	
	// accept the connection request
	struct sockaddr_in conClientAdd;
	socklen_t clientAddLength = sizeof(conClientAdd);
	while (1)
	{
		FD_ZERO(&read_fd_set);
		// set the fd_set before passing it to the select all
		for (i = 0; i < MAX_CONNECTION; i++)
		{
			if (all_connection[i] >= 0)
				FD_SET(all_connection[i], &read_fd_set);
		}

		int ret_val = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
		if (ret_val >= 0)
		{
			std::cout << "Select returned with " << ret_val << std::endl;
			if (FD_ISSET(serverSD, &read_fd_set))
			{
				std::cout << "Returned SD is " << serverSD << std::endl;
				memset(&conClientAdd, 0, sizeof(conClientAdd));
				int conSocket = accept(serverSD, (struct sockaddr*)&conClientAdd, &clientAddLength);
				if (conSocket < 0)
				{
					std::cout << "failed to accept connection to request" << std::endl;
					continue;
				}
				std::cout << "new client with is connected" << std::endl;
				for (i = 0; i < MAX_CONNECTION; i++)
				{
					if (all_connection[i] < 0)
					{
						all_connection[i] = conSocket;
						break;
					}
				}
				ret_val--;
				if(!ret_val)
					continue;
			}

			// Check if the SD with event is non-server sd
			char buffer[BUFFER_SIZE];
			for (i = 0; i < MAX_CONNECTION; i++)
			{
				if (all_connection[i] > 0 && (FD_ISSET(all_connection[i], &read_fd_set)))
				{
					std::cout << "Returned sd id " << all_connection[i] << std::endl;
					memset(buffer, 0, strlen(buffer));
					ret_val = recv(all_connection[i], buffer, BUFFER_SIZE, 0);
					if ((buffer[0] == 'q' || buffer[0] == 'Q'))
					{
						close(all_connection[i]);
						break;
					}
					std::cout << buffer << std::endl;
					send(all_connection[i], buffer, BUFFER_SIZE, 0);
				}
				ret_val--;
				if (!ret_val) continue;
			}
		}
	}
	for (i = 0; i < MAX_CONNECTION; i++)
	{
		if (all_connection[i] > 0)
			close(all_connection[i]);
	}
	return (0);
}

