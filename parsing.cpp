/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:07:13 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/04/03 12:59:33 by hkasbaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Server.hpp"
void ft_exit(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}
int has_space_in_front(const std::string& str) {
    if(str[0] == ' ')
        return 1;
    return 0;
}
std::string trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(str[start])) 
        start++;

    while (end > start && std::isspace(str[end - 1])) 
        end--;

    return str.substr(start, end - start);
}
std::string trim_tabs(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && str[start] == '\t') 
        start++;

    return str.substr(start, end - start);
}
void Server::display_server()
{
    std::cout << "host: " << host << std::endl;
    std::cout << "port: " << port << std::endl;
    std::cout << "server_names: " << std::endl;
    for (size_t i = 0; i < server_names.size(); i++)
        std::cout << "	" <<server_names[i] << std::endl;
    std::cout << "default_server: " << default_server << std::endl;
    std::cout << "error_pages: " << std::endl;
    for (std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
        std::cout << it->first << " => " << it->second << std::endl;
    std::cout << "client_body_size: " << client_body_size << std::endl;
}
void Route::clear_route()
{
    path.clear();
    methods.clear();
    directory.clear();
    redirect.clear();
    file.clear();
    directory_listing = false;
}
void Server::clear_server()
{
    host.clear();
    port = -1;
    server_names.clear();
    default_server = false;
    error_pages.clear();
    client_body_size.clear();
}

Server::Server(): port(-1), default_server(false),client_body_size("null")
{}
int get_count(std::string str,char token,int all)
{
    int tabcount = 0;
    if(all == 0)
        for (size_t l = 0; str[l] != '\0' && str[l] == '\t'; l++) 
            tabcount++;
    else if(all == 1)
        {for (size_t l = 0; str[l] != '\0'; l++) 
            if(str[l] == token)
                tabcount++;}
    else
    {
        bool insideQuotes = false;
        for (size_t l = 0; str[l] != '\0'; l++)
        {
            if (str[l] == '"') 
                insideQuotes = !insideQuotes;
            else if (str[l] == ':' && !insideQuotes) 
                tabcount++;
        }
    }
    return tabcount;
}
void display(std::pair<std::string, std::vector<std::string> > pair)
{
    std::cout << "first:" <<  pair.first << std::endl;
    if(pair.second.size() == 0)
        return;
    else
    {
        std::cout << "secend::" << std::endl;
        for (size_t i = 0; i < pair.second.size(); i++)
            std::cout << pair.second[i] << std::endl;
    }
}
void split_router_helper()
{

}
vecOfvecOfPair split_router(std::vector<std::vector<std::string> > big_vec)
{
    std::pair<std::string, std::vector<std::string> > pair;
    std::vector<std::pair<std::string, std::vector<std::string> > > ll;
    vecOfvecOfPair routerInfoSegments;
    for (size_t i = 0; i < big_vec.size(); i++)
    {
        for (size_t j = 0; j < big_vec[i].size(); j++)
        {
            int tabcount = 0;
            pair.first = big_vec[i][j];
            if (big_vec[i][j].find("router") != std::string::npos)
            {
                j++;
                tabcount = get_count(big_vec[i][j],'\t',0);
                while (tabcount == get_count(big_vec[i][j],'\t',0) && j < big_vec[i].size() - 1 )
                {

                    if(tabcount < get_count(big_vec[i][j],'\t',0))
                        ft_exit("router::error tab");
                    pair.second.push_back(big_vec[i][j]);
                    j++;
                }
                if(tabcount < get_count(big_vec[i][j],'\t',0))
                        ft_exit("router::error tab");
                if(tabcount == get_count(big_vec[i][j],'\t',0))
                    pair.second.push_back(big_vec[i][j++]);
                j--;
            }
            if (big_vec[i][j].find("error_pages") != std::string::npos)
            {
                j++;
                tabcount = get_count(big_vec[i][j],'\t',0);
                while (tabcount == get_count(big_vec[i][j],'\t',0) && j < big_vec[i].size() - 1 )
                {
                    if(tabcount < get_count(big_vec[i][j],'\t',0))
                        ft_exit("router::error tab");
                    pair.second.push_back(big_vec[i][j]);
                    j++;
                }
                if(tabcount < get_count(big_vec[i][j],'\t',0))
                        ft_exit("router::error tab");
                if(tabcount == get_count(big_vec[i][j],'\t',0))
                    pair.second.push_back(big_vec[i][j++]);
                j--;
            }
            ll.push_back(pair);
            pair.second.clear();
            pair.first.clear();
        }
        routerInfoSegments.push_back(ll);
        ll.clear();
    }
    return routerInfoSegments;
}
bool isAllWhitespace(const std::string& str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) 
        if (!isspace(*it)) 
            return false;
    return true;
}
std::vector<std::string> split_stream(std::string str,char token)
{
    std::istringstream iss(str);
    std::vector<std::string> results;
    std::string temp;
    while (std::getline(iss, temp, token)) 
        if (!temp.empty()) 
            results.push_back(trim(temp));
    return results;
}
void check_errors(std::string str,std::string error)
{
    
}
void host_pars(Server &sv,std::string line)
{
    if(!sv.host.empty())
        ft_exit("host::error agian host");
    std::string info;
    info = line.substr(line.find(":") + 1);
      std::vector<std::string> resl;
    resl = split_stream(info,' ');
    if(resl.size() == 0 || resl.size() > 1)
        ft_exit("host::error ktar mn 1");
    if(resl[0].find("localhost") != std::string::npos)
        sv.host = resl[0];
    else
    {
        if(resl[0].find_first_not_of("0123456789.") != std::string::npos)
            ft_exit("host::error alphabetic");
        if(get_count(resl[0],'.',1) != 3)
            ft_exit("host::error .");
        resl = split_stream(resl[0],'.');
        for (size_t i = 0; i < resl.size(); i++)
        {
            if(resl[i].find_first_not_of("0123456789") != std::string::npos)
                ft_exit("host::error alphabetic");
            if(resl[i][0] == '0')
                ft_exit("host::error 0");
            if(std::stoi(resl[i]) < 0 || std::stoi(resl[i]) > 255)
                ft_exit("host::error range");
        }
        sv.host = info;
    }
}	
void port_pars(Server &sv,std::string line)
{
    if(sv.port != -1)
        ft_exit("port:: error agian port");
    std::string info;
    info = line.substr(line.find(":") + 1);
    std::vector<std::string> resl;
    resl = split_stream(info,' ');
    if(resl.size() == 0 || resl.size() > 1)
        ft_exit("port:: error ktar mn 1");
    if(resl[0].find_first_not_of("0123456789") != std::string::npos)
        ft_exit("port:: error alphabetic");
    if(std::stoi(resl[0]) < 0 || std::stoi(resl[0]) > 65535)
        ft_exit("port:: error range");
    sv.port = std::stoi(resl[0]);
}

void server_name_pars(Server &sv,std::string line)
{
    std::string info;
    info = line.substr(line.find(":") + 1);
    std::vector<std::string> resl;
    resl = split_stream(info,',');
    for (size_t i = 0; i < resl.size(); i++)
        resl[i] = trim(resl[i]);
    if(resl.size() == 0)
        ft_exit("server_name:: error nothing");
    for (size_t i = 0; i < resl.size(); i++)
        if(resl[i].find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-.") != std::string::npos)
            ft_exit("server_name:: error alphabetic");
    for (size_t i = 0; i < resl.size(); i++)
        sv.server_names.push_back(resl[i]);
}
void client_body_size_pars(Server &sv,std::string line)
{
    std::string info;
    info = line.substr(line.find(":") + 1);
    std::vector<std::string> resl;
    resl = split_stream(info,' ');
    if(resl.size() == 0 || resl.size() > 1)
        ft_exit("client_body:: error ktar mn 1");
    if(resl[0].find_first_not_of("0123456789") != std::string::npos)
        ft_exit("client_body:: error alphabetic");
    if(std::stoi(resl[0]) < 0)
        ft_exit("client_body:: error range");
    sv.client_body_size = resl[0];
}
void error_pages_pars(Server &sv,std::vector<std::string> infos)
{	
    for (size_t i = 0; i < infos.size(); i++)
    {
        std::string first_part;
        std::string seceond_part;
        seceond_part = trim(infos[i].substr(infos[i].find(":") + 1));
        first_part = trim(infos[i].substr(0,infos[i].find(":")));
        if(first_part.find_first_not_of("0123456789") != std::string::npos)
            ft_exit("error_pages:: error alphabetic");
        if(std::stoi(first_part) < 100 || std::stoi(first_part) > 599)
            ft_exit("error_pages:: error range");
        if(seceond_part.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-./\"") != std::string::npos)
            ft_exit("error_pages:: error alphabetic");
        if(get_count(seceond_part,'\"',1) != 2)
            ft_exit("error_pages:: error \"");
        if(seceond_part[0] != '\"' || seceond_part[seceond_part.size() - 1] != '\"')
            ft_exit("error_pages:: error \"");
        sv.error_pages[std::stoi(first_part)] = seceond_part;
    }
}
void router_pars(Server &sv,std::vector<std::string> infos)
{
    Route route;
    std::vector<std::string> methods;
    methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("DELETE");

    for (size_t i = 0; i < infos.size(); i++)
    {
        if(infos[i].find("path:") != std::string::npos)
        {
            std::string info;
            info = trim(infos[i].substr(infos[i].find(":") + 1));
            if(info.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-./\"") != std::string::npos)
                ft_exit("router_path:: error alphabetic");
            if(get_count(info,'\"',1) != 2)
                ft_exit("router_path:: error \"");
            if(info[0] != '\"' || info[info.size() - 1] != '\"')
                ft_exit("router_path:: error \"");
            route.path = info;
        }
        else if(infos[i].find("methods:") != std::string::npos)
        {
            std::string line;
            int count_methods = 0;
            line = trim(infos[i].substr(infos[i].find(":") + 1));
            std::vector<std::string> resl;
            resl = split_stream(line,',');
            for (size_t i = 0; i < resl.size(); i++)
            {
                if(resl[i].find("GET") != std::string::npos)
                    count_methods++;
                else if(resl[i].find("POST") != std::string::npos)
                    count_methods++;
                else if(resl[i].find("DELETE") != std::string::npos)
                    count_methods++;
                else
                    ft_exit("router_methods:: error alphabetic");
            }
            if(resl.size() != count_methods)
                ft_exit("router_methods:: error no methods");
            count_methods = 0;
        }
        else if(infos[i].find("directory:") != std::string::npos)
        {
            std::string info;
            info = trim(infos[i].substr(infos[i].find(":") + 1));
            if(info.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-./\"") != std::string::npos)
                ft_exit("router_directory:: error alphabetic");
            if(get_count(info,'\"',1) != 2)
                ft_exit("router_directory:: error 1 \"");
            if(info[0] != '\"' || info[info.size() - 1] != '\"')
                ft_exit("router_directory:: error 2 \"");
            route.directory = info;
        }
        else if(infos[i].find("redirect:") != std::string::npos)
        {
            std::string info;
            info = trim(infos[i].substr(infos[i].find(":") + 1));
            if(info.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ:_-./\"") != std::string::npos)
                ft_exit("router_redirect:: error alphabetic");
            if(get_count(info,'\"',1) != 2)
                ft_exit("router_redirect:: error 1 \"");
            if(info[0] != '\"' || info[info.size() - 1] != '\"')
                ft_exit("router_redirect:: error 2 \"");
            route.redirect = info;
        }
        else if(infos[i].find("directory_listing:") != std::string::npos)
        {
            std::string info;
            info = trim(infos[i].substr(infos[i].find(":") + 1));
            if(info.find_first_not_of("abcdefjhigklmnopqrstuvwxyz") != std::string::npos)
                ft_exit("router_directory_listing:: error alphabetic");
            if(info != "on" && info != "off")
                ft_exit("router_directory_listing:: error on/off");
            if(info == "on")
                route.directory_listing = true;
            else
                route.directory_listing = false;
        }
        else
            ft_exit("router::error not valid key");
    }
    sv.routes.push_back(route);
}
void check_info_exit(std::vector<Server> s)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if(s[i].host.empty())
            ft_exit("Error::no host");
        if(s[i].port == -1)
            ft_exit("Error::no port");
        if(s[i].server_names.size() == 0)
            ft_exit("server_names::error");
        if(s[i].client_body_size == "null")
            ft_exit("client_body_size::error");
        if(s[i].routes.size() == 0)
            ft_exit("router::error");
    }
}
std::vector<Server> insert_data_to_server(vecOfvecOfPair server_router_info, Server &serv)
{
    std::vector<Server> servers;
    servers.resize(server_router_info.size());
    for (size_t i = 0; i < server_router_info.size(); i++)
    {
        for (size_t j = 0; j < server_router_info[i].size(); j++)
        {
            if(server_router_info[i][j].first.find("host") != std::string::npos)
                host_pars(serv, server_router_info[i][j].first);
            else if(server_router_info[i][j].first.find("port") != std::string::npos)
                port_pars(serv, server_router_info[i][j].first);
            else if(server_router_info[i][j].first.find("server_names") != std::string::npos)
                server_name_pars(serv, server_router_info[i][j].first);
            else if(server_router_info[i][j].first.find("client_body_size") != std::string::npos)
                client_body_size_pars(serv, server_router_info[i][j].first);
            else if(server_router_info[i][j].first.find("default_server") != std::string::npos)
                serv.default_server = true;
            else if(server_router_info[i][j].first.find("error_pages") != std::string::npos)
                error_pages_pars(serv, server_router_info[i][j].second);
            else if(server_router_info[i][j].first.find("router") != std::string::npos)
                router_pars(serv, server_router_info[i][j].second);
            else
                ft_exit("error::not valid key");
        }
        servers[i] = serv;
        serv.routes.clear();
        serv.clear_server();
    }

    return servers;
}
int main(int ac,char **av)
{
    std::string myText;
    int index = 0;
    std::ifstream MyReadFile(av[1]);
    std::vector<std::string > vct;
    std::vector<std::vector<std::string> > big_vec;
    size_t pos;
    std::pair<std::string, std::string> pair;
    bool inServerBlock = false;
    while (getline (MyReadFile, myText)) 
    {
        if(myText.empty() || isAllWhitespace(myText)) 
        	continue;
        if (myText.find("- server") != std::string::npos) 
        {
        	if (inServerBlock) 
        	{
        		big_vec.push_back(vct);
        		vct.clear();
        	}
        	inServerBlock = true;
        } 
        else if (inServerBlock) 
        {
            if(myText[myText.find(":")] != ':' || get_count(myText,':',3) != 1)
                ft_exit("error :");
        	if(get_count(myText,'\t',0) < 1 || has_space_in_front(trim_tabs(myText)) == 1)
                    ft_exit("error tabs or space or :");
        	vct.push_back(myText);
        }
    }
    if (!vct.empty()) 
        big_vec.push_back(vct);
    MyReadFile.close();
    vecOfvecOfPair server_router_info = split_router(big_vec);
    Server servers;
    std::vector<Server> all_info;
    all_info = insert_data_to_server(server_router_info, servers);
}