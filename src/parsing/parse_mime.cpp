#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

std::string get_mime_type(std::string file)
{
    std::map<std::string, std::string> mime_types;
    std::ifstream file1("../../mime.txt");
    std::string line;
    while (getline(file1, line))
    {
        std::string key = line.substr(0, line.find(" "));
        std::string value = line.substr(line.find("|") + 1);
        mime_types[key] = value;
    }

    std::string extension = file.substr(file.find_last_of(".") + 1);
    if (mime_types.find(extension) == mime_types.end())
        return "";
    std::cout << mime_types[extension] << std::endl;
    return mime_types[extension];
}
int main(int ac, char **av)
{
    std::string mime = get_mime_type(av[1]);
    return 0;
}