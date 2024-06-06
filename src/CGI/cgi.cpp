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
#include "../../incs/Cgi.hpp"

CGI::CGI(std::map<std::string, std::string> envi)
{
    this->_env = envi;
    _env["SERVER_NAME"] = "server";
    _env["SERVER_PORT"] = "8080";
    _env["REQUEST_METHOD"] = "GET";
    _env["PATH_INFO"] = "/Users/hkasbaou/Desktop/WebServeTeam/src/CGI";
    _env["SCRIPT_NAME"] = "/script.sh";
    _env["QUERY_STRING"] = "name=value1&password=value2";
    _env["CONTENT_TYPE"] = "text/html;";
    _env["CONTENT_LENGTH"] = "1";

}

char** CGI::set_env()
{
    char **env = new char*[_env.size() + 1];
    size_t i = 0;
    for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it)
    {
        std::string env_var = it->first + "=" + it->second;
        env[i] = new char[env_var.size() + 1];
        std::copy(env_var.begin(), env_var.end(), env[i]);
        env[i][env_var.size()] = '\0';
        i++;
    }
    env[i] = NULL;
    return env;
}
std::string CGI::check_extension_file(const std::string &file)
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

void CGI::exec_cpp(const std::string &path, char **env)
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
std::string CGI::exec_cgi()
{
    std::string output;
    std::string path = _env["PATH_INFO"] + _env["SCRIPT_NAME"];

    std::cout << "path::" << path << std::endl;
    int pipefd[2];
    // int pip_post[2];
    if (pipe(pipefd) == -1  /* || pipe(pip_post) == -1*/)
    {
        std::cerr << "Failed to create pipe." << std::endl;
        return "";
    }
    char **env = set_env();
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
        // close(pip_post[1]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1 /*|| dup2(pip_post[0], STDIN_FILENO) == -1*/)
        {
            std::cerr << "Failed to redirect STDOUT." << std::endl;
            close(pipefd[1]);
            // close(pip_post[0]);
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        // close(pip_post[0]);

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
        // close(pip_post[0]);
        // if(split_equal(env[2]) == "POST")
        // {
        //     write(pip_post[1], env, body.size());
        //     close(pip_post[1]);
        // }
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
    std::cout << "output::" << output << std::endl;
    return output;
}
int main(int ac, char **av)
{
    std::map<std::string, std::string> env;
    CGI cgi(env);
    // cgi.set_env();
    cgi.exec_cgi();
    // std::string out = exec_cgi();
    // std::cout << "outp:::" << out << std::endl;
    // return 0;
}