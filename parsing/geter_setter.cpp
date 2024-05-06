#include "Server.hpp"
// --------  Server  --------
// getters
std::string Server::get_host()
{
    return host;
}
std::string Server::get_root()
{
    return root;
}
int Server::get_port()
{
    return port;
}
std::vector<std::string> Server::get_server_names()
{
    return server_names;
}
bool Server::get_default_server()
{
    return default_server;
}
std::map<int, std::string> Server::get_error_pages()
{
    return error_pages;
}
std::string Server::get_client_body_size()
{
    return client_body_size;
}
// setters
void Server::set_host(std::string host)
{
    this->host = host;
}
void Server::set_root(std::string root)
{
    this->root = root;
}
void Server::set_port(int port)
{
    this->port = port;
}
void Server::set_server_names(std::vector<std::string> server_names)
{
    this->server_names = server_names;
}
void Server::set_default_server(bool default_server)
{
    this->default_server = default_server;
}
void Server::set_error_pages(int error_code, std::string error_page)
{
    this->error_pages[error_code] = error_page;
}
void Server::set_client_body_size(std::string client_body_size)
{
    this->client_body_size = client_body_size;
}
// --------  Route  --------
//getters
std::string Route::get_path()
{
    return path;
}
std::vector<std::string> Route::get_methods()
{
    return methods;
}
std::string Route::get_directory()
{
    return directory;
}

void Route::clear_route()
{
    path.clear();
    methods.clear();
    directory.clear();
    redirect.clear();
    file.clear();
    default_file = "index.html";
    directory_listing = false;
    cgi_bin.clear();
    cgi_extension.clear();
}
void Server::clear_server()
{
    host.clear();
    port = -1;
    server_names.clear();
    default_server = false;
    error_pages.clear();
    client_body_size.clear();
    root.clear();
    routes.clear();
}