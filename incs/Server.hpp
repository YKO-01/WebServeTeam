/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:07:22 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/03/26 14:07:23 by hkasbaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <map>
#include "Route.hpp"

typedef std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > vecOfvecOfPair;
class Server {
    private:

    public:
        std::string host;
        int port;
        std::vector<std::string> server_names;
        bool default_server;
        std::map<int, std::string> error_pages;
        std::string client_body_size;
        std::vector<Route> routes;
        Server();
        void clear_server();
        void display_server();
};