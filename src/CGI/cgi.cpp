#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include "../../incs/Config.hpp"

void set_env()
{}

void check_extention_file(std::string file)
{
    std::string extention;
    std::string formats[] = {"php", "sh", "cpp", "python"};
    std::string ext;
    size_t found = file.find_last_of(".");
    extention = file.substr(found + 1);
    ext = "";
    for (size_t i = 0; i < 4; i++)
    {
        if (extention == formats[i])
        {
            // std::cout << "::" << formats[i] << std::endl;
            ext = formats[i];
            break;
        }
    }
    if(ext == "")
    {
        std::cerr << "Error: invalid extention file" << std::endl;
        return;
    }
}
std::string CGI_EXEC(std::string full_path)
{
    char **env;
    env = new char *[9];
    env[0] = (char *)"SERVER_NAME=server";
    env[1] = (char *)"SERVER_PORT=8080";
    env[2] = (char *)"REQUEST_METHOD=GET";
    env[3] = (char *)"PATH_INFO=/Users/hkasbaou/Desktop/WebServeTeam/src/CGI";
    env[4] = (char *)"SCRIPT_NAME=/test.php";
    env[5] = (char *)"QUERY_STRING=name=value1&password=value2";
    env[6] = (char *)"CONTENT_TYPE=text/html;";
    env[7] = (char *)"CONTENT_LENGTH=512";

    env[8] = NULL;

    // /////////////////////////////
    std::string  output;
    std::string path = "./" + full_path;
    int pipefd[2];
    char *args[] = {(char *)path.c_str(), NULL};

    // check_extention_file(full_path);
    if(full_path[0] != '/')
        path = "./" + full_path;
    else
        path = full_path;
    if (pipe(pipefd) == -1)
    {
        std::cerr << "Failed to create pipe." << std::endl;
        return 0;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "Failed to fork." << std::endl;
        close(pipefd[0]);
        close(pipefd[1]);
        return 0;
    }
    else if (pid == 0)
    {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            std::cerr << "Failed to redirect STDOUT." << std::endl;
            close(pipefd[1]);
            return 0;
        }
        close(pipefd[1]);
        execve(args[0], args, env);
        std::cerr << "Error executing execve" << std::endl;
        exit(EXIT_FAILURE);
    }
    else 
    {
        close(pipefd[1]); 
        char buffer[1024];
        int bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRead] = '\0';
            output += buffer;
            memset(buffer, 0, sizeof(buffer));
        }
        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0); 
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "Error: failed to execute" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::string outputStr(output.begin(), output.end());
    }
    std::cout << "Output::" << output << std::endl;
    return output;
}
int main(int ac, char **av)
{
    if(ac < 2)
    {
        std::cerr << "Error: invalid argument" << std::endl;
        return 1;
    }
    CGI_EXEC(av[1]);
    return 0;
}

// int main(int ac,char **av)
// {
//     const char* phpPath = "/usr/bin/python";

//     // The PHP script you want to execute
//     const char* scriptPath = av[1];
//     char* const args[] = {(char*)phpPath, (char*)scriptPath, NULL};
//     char* const env[] = {NULL};
//     int pipefd[2];
//     if (pipe(pipefd) == -1)
//     {
//         std::cerr << "Failed to create pipe." << std::endl;
//         return 1;
//     }
//     pid_t pid = fork();

//     if (pid == -1)
//     {
//         std::cerr << "Failed to fork." << std::endl;
//         return 1;
//     }
//     else if (pid == 0)
//     {
//         close(pipefd[0]);
//         if (dup2(pipefd[1], STDOUT_FILENO) == -1)
//         {
//             std::cerr << "Failed to redirect STDOUT." << std::endl;
//             return 1;
//         }
//         close(pipefd[1]);
//         execve(phpPath, args, env);

//         std::cerr << "Error executing execve" << std::endl;
//         return 1;
//     }
//     else 
//     {
//         close(pipefd[1]); // Close unused write end

//         char buffer[1024];
//         std::vector<char> output;

//         int bytesRead;
//         while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
//             output.insert(output.end(), buffer, buffer + bytesRead);
//         close(pipefd[0]);
//         int status;
//         waitpid(pid, &status, 0);  // Wait for the child process to finish
//         std::string outputStr(output.begin(), output.end());
//         std::cout << "Output:\n" << outputStr << std::endl;
//     }
// }

