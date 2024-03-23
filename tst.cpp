#include <iostream>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct HTTP_Server {
    int socket;
    int port;
};

struct Route {
    std::string key;
    std::string value;
};

void init_server(HTTP_Server* http_server, int port);
void addRoute(std::map<std::string, std::string>& routes, const std::string& key, const std::string& value);

int main() {
    // initiate HTTP_Server
    HTTP_Server http_server;
    init_server(&http_server, 6969);

    int client_socket;

    // registering Routes
    std::map<std::string, std::string> routes;
    addRoute(routes, "/", "index.html");
    addRoute(routes, "/about", "about.html");

    printf("\n====================================\n");
    printf("=========ALL AVAILABLE ROUTES========\n");
    // display all available routes
    std::map<std::string, std::string>::iterator it;
    for (it = routes.begin(); it != routes.end(); ++it) {
        std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
    }

    while (true) {
        char client_msg[4096] = "";

        client_socket = accept(http_server.socket, NULL, NULL);

        read(client_socket, client_msg, 4095);
        printf("%s\n", client_msg);

        // parsing client socket header to get HTTP method, route
        // char *method = nullptr;
        // char *urlRoute = nullptr;

        // char *client_http_header = strtok(client_msg, "\n");

        // printf("\n\n%s\n\n", client_http_header);

        // char *header_token = strtok(client_http_header, " ");

        // int header_parse_counter = 0;
        // while (header_token != nullptr) {
        //     switch (header_parse_counter) {
        //         case 0:
        //             method = header_token;
        //             break;
        //         case 1:
        //             urlRoute = header_token;
        //             break;
        //     }
        //     header_token = strtok(NULL, " ");
        //     header_parse_counter++;
        // }
        // std::cout << "Method: " << method << std::endl;
        // std::cout << "Route : " << urlRoute << std::endl;
        // char tmplt[100] = "";

        // if (strstr(urlRoute, "/static/") != NULL) {
        //     strcat(tmplt, "static/index.css");
        // } else {
        //     // Assuming `routes` is your std::map containing routes
        //     std::map<std::string, std::string>::iterator destination = routes.find(urlRoute);            
        //     strcat(tmplt, "templates/");
        //     if (destination == routes.end()) {
        //         strcat(tmplt, "404.html");
        //     } else {
        //         strcat(tmplt, destination->second.c_str());
        //     }
        // }
        // std::cout << "Template: " << tmplt << std::endl;
    }

    return 0;
}

void init_server(HTTP_Server* http_server, int port) {
    http_server->port = port;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    listen(server_socket, 5);

    http_server->socket = server_socket;
    printf("HTTP Server Initialized\nPort: %d\n", http_server->port);
}

void addRoute(std::map<std::string, std::string>& routes, const std::string& key, const std::string& value) {
    if (routes.find(key) != routes.end()) {
        std::cout << "============ WARNING ============\n";
        std::cout << "A Route For \"" << key << "\" Already Exists\n";
    } else {
        routes[key] = value;
    }
}

// /////////////
// #include <iostream>
// #include <fstream>

// #include <iostream>
// #include <fstream>

// char* render_static_file(const char* fileName) {
//     std::ifstream file(fileName, std::ios::binary | std::ios::ate);

//     if (!file.is_open()) {
//         return nullptr;
//     } else {
//         std::cout << fileName << " does exist" << std::endl;
//     }
//     std::cout << "stopeed" << std::endl;
//     std::streampos fileSize = file.tellg();
//     file.seekg(0, std::ios::beg);

//     // Explicit conversion to integer to resolve ambiguity
//     int fileSizeInt = static_cast<int>(fileSize);
//     char* temp = new char[fileSizeInt + 1];
//     file.read(temp, fileSize);
//     temp[fileSizeInt] = '\0';

//     file.close();
//     return temp;
// }

// int main() {
//     char* file = render_static_file("gg1.html");
//     if (file == nullptr) {
//         std::cout << "File Not Found" << std::endl;
//     } else {
//         std::cout << file << std::endl;
//     }
//     return 0;
// }