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
#include <map>
#define BUFFER_SIZE 1024
#define MAX_CONNECTION 5
//  write funtion that parse query string and save it env variable to send it to execve
void exec(std::string request)
{
    std::cout << "request:" << request << std::endl;
    //return from request until first /n
    std::string path = request.substr(0, request.find("\n"));
    std::cout << "path:" << path << std::endl;
    // split path nad it secend by 
    std::string file = path.substr(5, path.find(" ", 5) - 5);
    std::cout << "file:" << file << std::endl;
    //remove query string from file it start from ? to end and save query string in new variable
    std::string query_string = "";
    if(file.find("?") != std::string::npos)
    {
        query_string = file.substr(file.find("?") + 1);
        file = file.substr(0, file.find("?"));
    }
    std::cout << "file:" << file << std::endl;
    std::cout << "query_string:" << query_string << std::endl;




    // 
    std::string full_path = "." + std::string(file);
    std::cout << "full_path:" << full_path <<  std::endl;
    // // // std::vector<char> output;
    // std::string  output;
    // char *args[] = {(char *)full_path.c_str(), NULL};
    // int pipefd[2];
    // if (pipe(pipefd) == -1)
    // {
    //     std::cerr << "Failed to create pipe." << std::endl;
    //     return ;
    // }
    // pid_t pid = fork();
    // if (pid == -1)
    // {
    //     std::cerr << "Failed to fork." << std::endl;
    //     return ;
    // }
    // else if (pid == 0)
    // {
    //     close(pipefd[0]);
    //     if (dup2(pipefd[1], STDOUT_FILENO) == -1)
    //     {
    //         std::cerr << "Failed to redirect STDOUT." << std::endl;
    //         return ;
    //     }
    //     close(pipefd[1]);
    //     // execve(phpPath, args, env);
    //     execve(args[0], args, NULL);
    //     std::cerr << "Error executing execve" << std::endl;
    //     return ;
    // }
    // else 
    // {
    //     close(pipefd[1]); // Close unused write end
    //     char buffer[1024];
    //     int bytesRead;
    //     while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
    //     {
    //         output += buffer;
    //         // std::cout << "bytesRead: " << bytesRead << std::endl;
    //         // output.insert(output.end(), buffer, buffer + bytesRead);
    //         memset(buffer, 0, sizeof(buffer));
    //     }
    //     close(pipefd[0]);
    //     int status;
    //     waitpid(pid, &status, 0);  // Wait for the child process to finish
    //     std::string outputStr(output.begin(), output.end());
    //     // std::cout << "Output:\n" << outputStr << std::endl;
    // }
    // std::cout << "Output::" << output << std::endl;
}

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
                    exec(recieveData);
                    // std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
                    // send(conection[i], httpResponse.c_str(), httpResponse.size(), 0);
                }
            }
        }
    }
}
