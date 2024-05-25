/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:07:13 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/05/25 10:05:07 by hkasbaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "../../incs/Config.hpp"


void Config::display_server()
{
    std::cout << "host: " << host << std::endl;
    std::cout << "port: " << port << std::endl;
    std::cout << "root: " << root << std::endl;
    std::cout << "server_names: " << std::endl;
    for (size_t i = 0; i < server_names.size(); i++)
        std::cout << "	" <<server_names[i] << std::endl;
    std::cout << "default_server: " << default_server << std::endl;
    std::cout << "error_pages: " << std::endl;
    for (std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
        std::cout << it->first << " => " << it->second << std::endl;
    std::cout << "client_body_size: " << client_body_size << std::endl;
}


Config::Config(): port(-1), default_server(false),client_body_size("null")
{}

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

std::string remove_quots(std::string str)
{
    std::string res;
    for (size_t i = 0; i < str.size(); i++)
    {
        if(str[i] != '\"')
            res += str[i];
    }
    return res;
}
std::string trim_and_check_exist(std::string data,std::string error,int check_cots)
{
    std::string info;
    info = trim(data.substr(data.find(":") + 1));
    if(info.size() == 0)
        ft_exit(error + "nothing");
    if(check_cots == 1)
    {
        if(get_count(info,'\"',1) != 2)
            ft_exit(error + "\"");
        if(info[0] != '\"' || info[info.size() - 1] != '\"')
            ft_exit(error + "\"");
    }
    return info;
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
            if (big_vec[i][j].find("- router") != std::string::npos)
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
            if (big_vec[i][j].find("- error_pages") != std::string::npos)
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


// void check_errors(std::string str,std::string error)
// {
    
// }
void host_pars(Config &sv,std::string line)
{
    if(!sv.get_host().empty())
        ft_exit("host::error agian host");
    std::string info;
    info = line.substr(line.find(":") + 1);
      std::vector<std::string> resl;
    resl = split_stream(info,' ');
    if(resl.size() == 0 || resl.size() > 1)
        ft_exit("host::error ktar mn 1");
    if(resl[0].find("localhost") != std::string::npos)
        sv.set_host(resl[0]);
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
        sv.set_host(info);
    }
}	
void port_pars(Config &sv,std::string line)
{
    if(sv.get_port() != -1)
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
    sv.set_port(std::stoi(resl[0]));
}
void root_pars(Config &sv,std::string line)
{
    std::string info;
    info = line.substr(line.find(":") + 1);
    std::vector<std::string> resl;
    resl = split_stream(info,' ');
    std::cout << "size::" << resl.size() << std::endl;
    if(resl.size() == 0 || resl.size() > 1)
        ft_exit("root:: error ktar mn 1");
    if(resl[0].find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-./\"") != std::string::npos)
        ft_exit("root:: error alphabetic");
    if(get_count(resl[0],'\"',1) != 2)
        ft_exit("root:: error \"");
    if(resl[0][0] != '\"' || resl[0][resl[0].size() - 1] != '\"')
        ft_exit("root:: error \"");
    sv.set_root(remove_quots(resl[0]));

}
void server_name_pars(Config &sv,std::string line)
{
    std::string info;
    std::vector<std::string> server_names1;
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
        server_names1.push_back(resl[i]);
    sv.set_server_names(server_names1);
}
void client_body_size_pars(Config &sv,std::string line)
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
    sv.set_client_body_size(resl[0]);
}
void error_pages_pars(Config &sv,std::vector<std::string> infos)
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
        sv.set_error_pages(std::stoi(first_part),remove_quots(seceond_part));
    }
}

void router_pars(Config &sv,std::vector<std::string> infos)
{
    Route route;
    std::vector<std::string> methods;
    methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("DELETE");
    route.clear_route();
    for (size_t i = 0; i < infos.size(); i++)
    {
        if(infos[i].find("default_file:") != std::string::npos)
        {
            std::string info = trim_and_check_exist(infos[i],"router_default_file:: error ",0);
            // info = trim(infos[i].substr(infos[i].find(":") + 1));
            // if(info.size() == 0)
            //     ft_exit("router_default_file:: error nothing");
            route.set_default_file(info);
        }
        else if(infos[i].find("path:") != std::string::npos)
        {
            std::string info = trim_and_check_exist(infos[i],"router_path:: error ",1);
            // info = trim(infos[i].substr(infos[i].find(":") + 1));
            // if(info.size() == 0)
            //     ft_exit("router_path:: error nothing");
            if(info.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-./\"") != std::string::npos)
                ft_exit("router_path:: error alphabetic");
            // if(get_count(info,'\"',1) != 2)
            //     ft_exit("router_path:: error \"");
            // if(info[0] != '\"' || info[info.size() - 1] != '\"')
            //     ft_exit("router_path:: error \"");
            route.set_path(remove_quots(info));
        }
        else if(infos[i].find("methods:") != std::string::npos)
        {
            std::string line;
            std::vector<std::string> mtods;
            int count_methods = 0;
            line = trim(infos[i].substr(infos[i].find(":") + 1));
            std::vector<std::string> resl;
            resl = split_stream(line,',');
            if(resl.size() == 0)
                ft_exit("router_methods:: error nothing");
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
            if(resl.size() != (size_t)count_methods)
                ft_exit("router_methods:: error no methods");
            count_methods = 0;
            for (size_t i = 0; i < resl.size(); i++)
                mtods.push_back(resl[i]);
            route.set_methods(mtods);
        }
        else if(infos[i].find("directory:") != std::string::npos)
        {
            std::string info = trim_and_check_exist(infos[i],"router_directory:: error ",1);
            // info = trim(infos[i].substr(infos[i].find(":") + 1));
            if(info.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-./\"") != std::string::npos)
                ft_exit("router_directory:: error alphabetic");
            // if(get_count(info,'\"',1) != 2)
            //     ft_exit("router_directory:: error 1 \"");
            // if(info[0] != '\"' || info[info.size() - 1] != '\"')
            //     ft_exit("router_directory:: error 2 \"");
            route.set_directory(remove_quots(info));
        }
        else if(infos[i].find("redirect:") != std::string::npos)
        {
            std::string info = trim_and_check_exist(infos[i],"router_redirect:: error ",1);
            // info = trim(infos[i].substr(infos[i].find(":") + 1));
            if(info.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ:_-./\"") != std::string::npos)
                ft_exit("router_redirect:: error alphabetic");
            // if(get_count(info,'\"',1) != 2)
            //     ft_exit("router_redirect:: error 1 \"");
            // if(info[0] != '\"' || info[info.size() - 1] != '\"')
            //     ft_exit("router_redirect:: error 2 \"");
            route.set_redirect(remove_quots(info));
        }
        else if(infos[i].find("directory_listing:") != std::string::npos)
        {
            std::string info = trim_and_check_exist(infos[i],"router_directory_listing:: error ",0);
            // info = trim(infos[i].substr(infos[i].find(":") + 1));
            // if(info.find_first_not_of("abcdefjhigklmnopqrstuvwxyz") != std::string::npos)
            //     ft_exit("router_directory_listing:: error alphabetic");
            if(info != "on" && info != "off")
                ft_exit("router_directory_listing:: error on/off");
            if(info == "on")
                route.set_directory_listing(true);
            else
                route.set_directory_listing(false);
        }
        else if(infos[i].find("useCGI:") != std::string::npos)
        {
            std::string info = trim_and_check_exist(infos[i],"router_useCGI:: error ",0);
            // info = trim(infos[i].substr(infos[i].find(":") + 1));
            // if(info.find_first_not_of("abcdefjhigklmnopqrstuvwxyz") != std::string::npos)
            //     ft_exit("router_useCGI:: error alphabetic");
            if(info != "true" && info != "false")
                ft_exit("router_useCGI:: error on/off");
            if(info == "true")
                route.set_useCGI(true);
            else
                route.set_useCGI(false);
        }
        // else if(infos[i].find("cgi_bin:") != std::string::npos)
        // {
        //     std::string info = trim_and_check_exist(infos[i],"router_cgi_bin:: error ",0);
        //     // info = trim(infos[i].substr(infos[i].find(":") + 1));
        //     // if(info.size() == 0)
        //     //     ft_exit("router_cgi_bin:: error nothing");
        //     route.set_cgi_bin(info);
        // }
        // else if(infos[i].find("cgi_extension:") != std::string::npos)
        // {
        //     std::string info = trim_and_check_exist(infos[i],"router_cgi_extension:: error ",0);
        //     // info = trim(infos[i].substr(infos[i].find(":") + 1));
        //     // if(info.size() == 0)
        //     //     ft_exit("router_cgi_extension:: error nothing");
        //     route.set_cgi_extension(info);
        // }
        else
            ft_exit("router::error not valid key");
    }
    sv.set_routes(route);
}
void check_info_exit(std::vector<Config> s)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if(s[i].get_host().empty())
            ft_exit("Error::no host");
        if(s[i].get_port() == -1)
            ft_exit("Error::no port");
        if(s[i].get_server_names().size() == 0)
            ft_exit("server_names::error");
        if(s[i].get_client_body_size() == "null")
            ft_exit("client_body_size::error");
        if(s[i].get_routes().size() == 0)
            ft_exit("router::error");
    }
}
std::vector<Config> insert_data_to_server(vecOfvecOfPair server_router_info, Config &serv)
{
    std::vector<Config> servers;
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
            else if(server_router_info[i][j].first.find("root") != std::string::npos)
                root_pars(serv, server_router_info[i][j].first);
            else if(server_router_info[i][j].first.find("default_server") != std::string::npos)
                serv.set_default_server(true);
            else if(server_router_info[i][j].first.find("error_pages") != std::string::npos)
                error_pages_pars(serv, server_router_info[i][j].second);
            else if(server_router_info[i][j].first.find("router") != std::string::npos)
                router_pars(serv, server_router_info[i][j].second);
            else
                ft_exit("error::not valid key");
        }
        servers[i] = serv;
        serv.get_routes().clear();
        serv.clear_server();
    }

    return servers;
}
void display_info(std::vector<Config> all_info)
{
    for (size_t i = 0; i < all_info.size(); i++)
    {
        std::cout << "-------------------------------" << std::endl;
        all_info[i].display_server();
        for (size_t j = 0; j < all_info[i].get_routes().size(); j++)
        {
            std::vector<std::string> methods = all_info[i].get_routes()[j].get_methods();
            std::cout << "path: " << all_info[i].get_routes()[j].get_path() << std::endl;
            std::cout << "default_file: " << all_info[i].get_routes()[j].get_default_file() << std::endl;
            std::cout << "methods: " << std::endl;
            for (size_t k = 0; k < methods.size(); k++)
                std::cout << "	" << methods[k] << std::endl;
            std::cout << "directory: " << all_info[i].get_routes()[j].get_directory() << std::endl;
            std::cout << "redirect: " << all_info[i].get_routes()[j].get_redirect() << std::endl;
            std::cout << "directory_listing: " << all_info[i].get_routes()[j].get_directory_listing() << std::endl;
            std::cout << "useCGI: " << all_info[i].get_routes()[j].get_useCGI() << std::endl;
            // std::cout << "cgi_bin: " << all_info[i].get_routes()[j].get_cgi_bin() << std::endl;
            // std::cout << "cgi_extension: " << all_info[i].get_routes()[j].get_cgi_extension() << std::endl;
        }
    }
}
void	Config::parssConfigs(char **av)
{
    std::string myText;
    std::ifstream MyReadFile(av[1]);
    std::vector<std::string > vct;
    std::vector<std::vector<std::string> > big_vec;
    std::pair<std::string, std::string> pair;
    bool inServerBlock = false;
    while (getline (MyReadFile, myText)) 
    {
        if(myText.empty() || isAllWhitespace(myText) || trim(myText).at(0) == '#')
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
    Config servers;
    all_info = insert_data_to_server(server_router_info, servers);
    display_info(all_info);
}

int main(int argc, char const *argv[])
{
    Config c;
    c.parssConfigs((char **)argv);
    return 0;
}
