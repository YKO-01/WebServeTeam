#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#define BUFFER_SIZE 1024
void	Read_Routine(int sock, char *buf)
{
	while (1)
	{
		recv(sock, buf, BUFFER_SIZE, 0);
		if (strlen(buf) <= 0)
		{
			close(sock);
			return ;
		}
		std::cout << "recieve from server : " << buf << std::endl;
		memset(buf, 0, BUFFER_SIZE);
	}
}
void	Write_Routine(int sock, char *buf)
{
	while (1)
	{
		char *buff;
		std::cin.getline(buf, BUFFER_SIZE);
		send(sock, buf, BUFFER_SIZE, 0);
		memset(buf, 0, BUFFER_SIZE);
	}
}
int main(int argc, char const **argv)
{
    int s_fd;
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }
    std::cout << "Socket created successfully:: " << s_fd << std::endl;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5555);
    server.sin_addr.s_addr = INADDR_ANY;
    if(connect(s_fd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }
    int pid = fork();
	if (pid == 0)
		Write_Routine(s_fd, new char[BUFFER_SIZE]);
	else
		Read_Routine(s_fd, new char[BUFFER_SIZE]);
    return 0;
}
