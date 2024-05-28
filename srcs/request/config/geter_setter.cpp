/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geter_setter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 14:32:35 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/05/27 09:40:53 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
// #include "../../incs/Route.hpp"

// --------  config  --------
// getters
std::string Config::get_host()
{
    return host;
}
std::string Config::get_root()
{
    return root;
}
int Config::get_port()
{
    return port;
}
std::vector<std::string> Config::get_server_names()
{
    return server_names;
}
bool Config::get_default_server()
{
    return default_server;
}
std::map<int, std::string> Config::get_error_pages()
{
    return error_pages;
}
size_t Config::get_client_body_size()
{
    return client_body_size;
}
std::vector<Config>&	Config::get_allInfo()
{
	return (all_info);
}	
std::vector<Route> Config::get_routes()
{
    return routes;
}
// setters
void Config::set_host(std::string host)
{
    this->host = host;
}
void Config::set_root(std::string root)
{
    this->root = root;
}
void Config::set_port(int port)
{
    this->port = port;
}
void Config::set_server_names(std::vector<std::string> server_names)
{
    this->server_names = server_names;
}
void Config::set_default_server(bool default_server)
{
    this->default_server = default_server;
}
void Config::set_error_pages(int error_code, std::string error_page)
{
    this->error_pages[error_code] = error_page;
}
void Config::set_client_body_size(size_t client_body_size)
{
    this->client_body_size = client_body_size;
}
void Config::set_routes(Route routes)
{
    this->routes.push_back(routes);
}
// --------  Route  --------
//getters
std::string Route::get_path()
{
    return path;
}
std::vector<Method> Route::get_methods()
{
    return methods;
}
std::string Route::get_directory()
{
    return directory;
}
std::string Route::get_redirect()
{
    return redirect;
}
std::string Route::get_file()
{
    return file;
}
std::string Route::get_default_file()
{
    return default_file;
}

bool Route::get_useCGI()
{
    return useCGI;
}
bool Route::get_directory_listing()
{
    return directory_listing;
}
//setters
void Route::set_path(std::string path)
{
    this->path = path;
}
void Route::set_methods(Method method)
{
    this->methods.push_back(method);
}
void Route::set_directory(std::string directory)
{
    this->directory = directory;
}
void Route::set_redirect(std::string redirect)
{
    this->redirect = redirect;
}
void Route::set_file(std::string file)
{
    this->file = file;
}
void Route::set_default_file(std::string default_file)
{
    this->default_file = default_file;
}
void Route::set_useCGI(bool cgi)
{
    this->useCGI = cgi;
}

void Route::set_directory_listing(bool directory_listing)
{
    this->directory_listing = directory_listing;
}

//methods


void Route::clear_route()
{
    path.clear();
    methods.clear();
    directory.clear();
    redirect.clear();
    file.clear();
    default_file = "index.html";
    directory_listing = false;
    useCGI = false;
    // cgi_bin.clear();
    // cgi_extension.clear();
}
void Config::clear_server()
{
    host.clear();
    port = -1;
    server_names.clear();
    default_server = false;
    error_pages.clear();
    client_body_size  = 0;
    root.clear();
    routes.clear();
}

Route::Route()
{
    directory_listing = false;
    useCGI = false;
    default_file = "index.html"; 
}
Route::~Route()
{
    path.clear();
    methods.clear();
    directory.clear();
    redirect.clear();
    file.clear();
    default_file.clear();
}
