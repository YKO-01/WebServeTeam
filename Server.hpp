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