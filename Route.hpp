#include <iostream>
#include <vector>
#include <map>
class Route {
private:
    

public:
    std::string path;
    std::vector<std::string> methods;
    std::string directory;
    std::string redirect;
    std::string file;
    bool directory_listing;
    
    void clear_route();
    // Add getters and setters as needed
};