/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:07:22 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/06/09 09:18:31 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Config_HPP
#define Config_HPP

#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Route.hpp"

enum cgi_extention
{
    php,
    sh,
    cpp,
    python,
};

typedef std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > vecOfvecOfPair;
class Config {
    private:
        std::string                 host;
        std::string                 root;
        int                         port;
        std::vector<std::string>    server_names;
        bool                        default_server;
        std::map<int, std::string>  error_pages;
        std::string                 client_body_size;
		std::vector<Config>         all_info;
        std::vector<Route>          routes;
    public:
        
        Config();
		

        //getters
        std::string get_host();
        std::string get_root();
        int get_port();
        std::vector<std::string> get_server_names();
        bool get_default_server();
        std::map<int, std::string> get_error_pages();
        std::string get_client_body_size();
        std::vector<Route> get_routes();
		// std::vector<Route> get_routes() ;
        std::vector<Config>& get_allInfo();
        
        // setters
        void set_host(std::string host);
        void set_root(std::string root);
        void set_port(int port);
        void set_server_names(std::vector<std::string> server_names);
        void set_default_server(bool default_server);
        void set_error_pages(int error_code, std::string error_page);
        void set_client_body_size(std::string client_body_size);
        void set_routes(Route routes);
        
        
        void clear_server();
        void display_server();
		void parssConfigs(char **av);
};
void ft_exit(std::string str);
int has_space_in_front(const std::string& str) ;
std::string trim(const std::string& str) ;
std::string trim_tabs(const std::string& str);
int get_count(std::string str,char token,int all);
bool isAllWhitespace(const std::string& str) ;
std::vector<std::string> split_stream(std::string str,char token); 


#endif
