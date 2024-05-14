#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>

int main(int ac, char **av)
{
    std::string path = "./" + std::string(av[1]);
    // std::vector<char> output;
    std::string  output;
    char *args[] = {(char *)path.c_str(), NULL};
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        std::cerr << "Failed to create pipe." << std::endl;
        return 1;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "Failed to fork." << std::endl;
        return 1;
    }
    else if (pid == 0)
    {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            std::cerr << "Failed to redirect STDOUT." << std::endl;
            return 1;
        }
        close(pipefd[1]);
        // execve(phpPath, args, env);
        execve(args[0], args, NULL);
        std::cerr << "Error executing execve" << std::endl;
        return 1;
    }
    else 
    {
        close(pipefd[1]); // Close unused write end
        char buffer[1024];
        int bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            output += buffer;
            // std::cout << "bytesRead: " << bytesRead << std::endl;
            // output.insert(output.end(), buffer, buffer + bytesRead);
            memset(buffer, 0, sizeof(buffer));
        }
        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0);  // Wait for the child process to finish
        std::string outputStr(output.begin(), output.end());
        // std::cout << "Output:\n" << outputStr << std::endl;
    }
    std::cout << "Output::" << output << std::endl;
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

