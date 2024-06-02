#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string>
#include<fstream>
#include<vector>
#include<cstddef>
#include<sstream>
#include "../../incs/Config.hpp"

void set_env()
{}
std::string check_extension_file(const std::string &file)
{
    std::string extension;
    std::string formats[] = {"php", "sh", "cpp", "python"};
    size_t found = file.find_last_of(".");
    if (found != std::string::npos)
    {
        extension = file.substr(found + 1);
        for (size_t i = 0; i < formats->size() ; i++)
        {
            if (extension == formats[i])
                return extension;
        }
    }
    return "";
}

std::string split_equal(const std::string &str)
{
    size_t pos = str.find("=");
    if (pos != std::string::npos)
        return str.substr(pos + 1);
    return "";
}

void exec_cpp(const std::string &path, char **env)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "Failed to fork." << std::endl;
        return;
    }
    else if (pid == 0)
    {
        const char* path_cpp = "/usr/bin/c++";
        const char* args[] = { "c++", path.c_str(), "-o", "a.out", NULL };
        execve(path_cpp, (char* const*)args, env);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "Error: failed to compile" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// std::string exec_cgi()
// {
//     char *env[] = {
//         (char *)"SERVER_NAME=server",
//         (char *)"SERVER_PORT=8080",
//         (char *)"REQUEST_METHOD=GET",
//         (char *)"PATH_INFO=/Users/hkasbaou/Desktop/WebServeTeam/src/CGI",
//         (char *)"SCRIPT_NAME=/script.cpp",
//         (char *)"QUERY_STRING=name=value1&password=value2",
//         (char *)"CONTENT_TYPE=text/html;",
//         (char *)"CONTENT_LENGTH=512",
//         NULL
//     };

//     std::string output;
//     std::string path = split_equal(env[3]) + split_equal(env[4]);

//     int pipefd[2];
//     if (pipe(pipefd) == -1)
//     {
//         std::cerr << "Failed to create pipe." << std::endl;
//         return "";
//     }

//     pid_t pid = fork();
//     if (pid == -1)
//     {
//         std::cerr << "Failed to fork." << std::endl;
//         close(pipefd[0]);
//         close(pipefd[1]);
//         return "";
//     }
//     else if (pid == 0)
//     {
//         close(pipefd[0]);
//         if (dup2(pipefd[1], STDOUT_FILENO) == -1)
//         {
//             std::cerr << "Failed to redirect STDOUT." << std::endl;
//             close(pipefd[1]);
//             exit(EXIT_FAILURE);
//         }
//         close(pipefd[1]);
//         std::string ext = check_extension_file(path);
//         if (ext == "cpp")
//         {
//             exec_cpp(path, env);
//             char *args[] = { (char *)"./a.out", NULL };
//             execve(args[0], args, env);
//         }
//         else
//         {
//             char *args[] = { (char *)path.c_str(), NULL };
//             execve(args[0], args, env);
//         }
//         perror("execve");
//         exit(EXIT_FAILURE);
//     }
//     else
//     {
//         close(pipefd[1]);
//         char buffer[1024];
//         int bytesRead;
//         while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
//         {
//             buffer[bytesRead] = '\0';
//             output += buffer;
//         }
//         close(pipefd[0]);
//         int status;
//         waitpid(pid, &status, 0);
//         if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
//         {
//             std::cerr << "Error: failed to execute" << std::endl;
//             return "";
//         }
//     }
//     return output;
// }
std::string exec_cgi()
{
    char *env[] = {
        (char *)"SERVER_NAME=server",
        (char *)"SERVER_PORT=8080",
        (char *)"REQUEST_METHOD=POST", // Change this to GET or POST as needed
        (char *)"PATH_INFO=/Users/hkasbaou/Desktop/WebServeTeam/src/CGI",
        (char *)"SCRIPT_NAME=/script.php",
        (char *)"QUERY_STRING=name=value1&password=value2", // Only for GET
        (char *)"CONTENT_TYPE=text/html;",
        (char *)"CONTENT_LENGTH=1", // Only for POST
        NULL
    };

    std::string output;
    std::string path = split_equal(env[3]) + split_equal(env[4]);

    int pipefd[2];
    int pip_post[2];
    if (pipe(pipefd) == -1 || pipe(pip_post) == -1)
    {
        std::cerr << "Failed to create pipe." << std::endl;
        return "";
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "Failed to fork." << std::endl;
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }
    else if (pid == 0)
    {
        close(pipefd[0]);
        close(pip_post[1]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1 || dup2(pip_post[0], STDIN_FILENO) == -1)
        {
            std::cerr << "Failed to redirect STDOUT." << std::endl;
            close(pipefd[1]);
            close(pip_post[0]);
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        close(pip_post[0]);

        std::string ext = check_extension_file(path);
        if (ext == "cpp")
        {
            exec_cpp(path, env);
            char *args[] = { (char *)"./a.out", NULL };
            execve(args[0], args, env);
        }
        else
        {
            char *args[] = { (char *)path.c_str(), NULL };
            execve(args[0], args, env);
        }
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipefd[1]);
        close(pip_post[0]);
        if(split_equal(env[2]) == "POST")
        {
            write(pip_post[1], "1", 1);
            close(pip_post[1]);
        }
        char buffer[1024];
        int bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRead] = '\0';
            output += buffer;
        }
        close(pipefd[0]);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "Error: failed to execute" << std::endl;
            return "";
        }
    }
    return output;
}
int main(int ac, char **av)
{
    std::string out = exec_cgi();
    std::cout << "outp:::" << out << std::endl;
    return 0;
}