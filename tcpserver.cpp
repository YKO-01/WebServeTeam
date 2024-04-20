#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <netinet/in.h>
#include <sys/select.h>
#include <poll.h>
#include <vector>
#define BUFFER_SIZE 1024
#define MAX_CONNECTION 5
int main(int ac, char const *av[])
{
    (void) ac;
	(void) av;
	int serverSD;
    fd_set read_fd_set;
	// create a socket for the server
	serverSD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSD < 0)
	{
		std::cout << "failed to creation a socket" << std::endl;
		exit(EXIT_FAILURE);
	}   
    int option = 1;
    setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));  
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(5555);

    if (bind(serverSD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cout << "binding failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	// listen to the client connection request
	if (listen(serverSD, 1) < 0)
	{
		std::cout << "failed to listening" << std::endl;
		exit(EXIT_FAILURE);
	}

    struct sockaddr_in conClientAdd;
    int conection[MAX_CONNECTION];
    for (size_t i = 0; i < MAX_CONNECTION; i++)
        conection[i] = -1;
    conection[0] = serverSD;
    while (1)
    {
        // int conSocket;
        // std::cout << "-==============================-" << std::endl;
        // FD_ZERO(&read_fd_set);
        // for (size_t i = 0; i < MAX_CONNECTION; i++)
        // {
        //     if(conection[i] >= 0)
        //     {
        //         FD_SET(conection[i], &read_fd_set);
        //     }
        // }
        // int ret_select ;
        // ret_select = select(FD_SETSIZE,&read_fd_set, NULL, NULL, NULL);
        // std::cout << "SELECT return::" << ret_select << std::endl;
        // if(ret_select > 0)
        // {
        //     std::cout << "FD_ISSET::> " << FD_ISSET(serverSD, &read_fd_set) << std::endl;
        //     if(FD_ISSET(serverSD, &read_fd_set))
        //     {
        //         std::cout << "NEW CONNECTION HERE " << std::endl;
        //         memset(&conClientAdd, 0, sizeof(conClientAdd));
        //         conSocket = accept(serverSD, (struct sockaddr*)&conClientAdd, (socklen_t*)&conClientAdd);
        //         std::cout << "conSocket:" << conSocket << std::endl;
        //         if(conSocket < 0)
        //             std::cout << "ERROR : ACCEPTING SOCKET" << std::endl;
        //         std::cout << "ACCEPTED CONNECTION" << std::endl;
        //         for (size_t i = 0; i < MAX_CONNECTION; i++)
        //         {
        //             if(conection[i] < 0)
        //             {
        //                 conection[i] = conSocket;
        //                 break;
        //             }
        //         }
        //         ret_select--;
        //         if(!ret_select)
        //             continue;
        //     }
        // }
        // for (size_t i = 0; i < MAX_CONNECTION; i++)
        // {
        //     if(conection[i] > 0 && FD_ISSET(conection[i], &read_fd_set))
        //     {
        //         char recieveData[BUFFER_SIZE];
        //         memset(recieveData, 0, strlen(recieveData));
        //         recv(conection[i], recieveData, BUFFER_SIZE, 0);
        //         std::cout << "REcive:" << recieveData << std::endl;
        //     }
        // }
        std::vector<struct pollfd> pollfds(MAX_CONNECTION);
        for (size_t i = 0; i < MAX_CONNECTION; i++)
        {
            pollfds[i].fd = conection[i];
            pollfds[i].events = POLLIN;
        }
        int ret_poll = poll(pollfds.data(), MAX_CONNECTION, -1);
        std::cout << "RETURN POLL::>" << ret_poll << std::endl;
        if(ret_poll < 0)
        {
            std::cout << "ERROR POLL" << std::endl;
            exit(EXIT_FAILURE);
        }
        else if (ret_poll == 0)
        {
            std::cout << "Timeout occurred in poll() " << std::endl;
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < MAX_CONNECTION; i++)
        {
            if(pollfds[i].revents & POLLIN)
            {
                if(conection[i] == serverSD)
                {
                    int conSocket;
                    memset(&conClientAdd, 0, sizeof(conClientAdd));
                    conSocket = accept(serverSD, (struct sockaddr*)&conClientAdd, (socklen_t*)&conClientAdd);
                    if(conSocket < 0)
                        std::cout << "ERROR : ACCEPTING SOCKET" << std::endl;
                    std::cout << "ACCEPTED CONNECTION" << std::endl;
                    for (size_t i = 0; i < MAX_CONNECTION; i++)
                    {
                        if(conection[i] < 0)
                        {
                            conection[i] = conSocket;
                            break;
                        }
                    }
                }
                else
                {
                    int ret_recv = 0;
                    char recieveData[BUFFER_SIZE];
                    memset(recieveData, 0, strlen(recieveData));
                    ret_recv =  recv(conection[i], recieveData, BUFFER_SIZE, 0);
                    if(ret_recv < 0)
                    {
                        std::cout << "ERROR RECV" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    else if(ret_recv == 0)
                    {
                        std::cout << "Connection closed" << std::endl;
                        close(conection[i]);
                        conection[i] = -1;
                        continue;
                    }
                    std::cout << "REcive:" << recieveData << std::endl;
                    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
                    send(conection[i], httpResponse.c_str(), httpResponse.size(), 0);
                }
            }
        }
    }
}
