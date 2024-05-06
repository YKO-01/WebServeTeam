/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:07:22 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/05/04 13:01:14 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include "Route.hpp"

typedef std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > vecOfvecOfPair;
class Config {
    private:
		std::vector<Config>	allConfigs;
    public:
        std::string host;
        int port;
        std::vector<std::string> server_names;
        bool default_server;
        std::map<int, std::string> error_pages;
        std::string client_body_size;
        std::vector<Route> routes;
        Config();
        void clear_server();
        void display_server();
		void	parssConfigs(char **av);
};


#endif
