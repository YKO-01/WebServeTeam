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
std::string check_extension_file(const std::string &file)
{
    std::string extension;
    std::string formats[] = {"php", "sh", "cpp", "python"};
    size_t found = file.find_last_of(".");
    if (found != std::string::npos)
    {
        extension = file.substr(found + 1);
        for (const auto& format : formats)
        {
            if (extension == format)
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
        const char* path_cpp = "/usr/bin/g++";
        const char* args[] = { "g++", path.c_str(), "-o", "a.out", nullptr };
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

std::string CGI_EXEC()
{
    char *env[] = {
        (char *)"SERVER_NAME=server",
        (char *)"SERVER_PORT=8080",
        (char *)"REQUEST_METHOD=GET",
        (char *)"PATH_INFO=/Users/hkasbaou/Desktop/WebServeTeam/src/CGI",
        (char *)"SCRIPT_NAME=/script.cpp",
        (char *)"QUERY_STRING=name=value1&password=value2",
        (char *)"CONTENT_TYPE=text/html;",
        (char *)"CONTENT_LENGTH=512",
        NULL
    };

    std::string output;
    std::string path = split_equal(env[3]) + split_equal(env[4]);
    std::cout << "path:: " << path << std::endl;

    int pipefd[2];
    if (pipe(pipefd) == -1)
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
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            std::cerr << "Failed to redirect STDOUT." << std::endl;
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);

        std::string ext = check_extension_file(path);
        if (ext == "cpp")
        {
            exec_cpp(path, env);
            char *args[] = { "./a.out", nullptr };
            execve(args[0], args, env);
        }
        else
        {
            char *args[] = { (char *)path.c_str(), nullptr };
            execve(args[0], args, env);
        }
        perror("execve");
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
        }
        close(pipefd[0]);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "Error: failed to execute" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cout << "Output::" << output << std::endl;
    return output;
}

int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cerr << "Error: invalid argument" << std::endl;
        return 1;
    }
    CGI_EXEC();
    return 0;
}