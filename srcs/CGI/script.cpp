#include<iostream>

int main()
{
    std::cout << "Content-type: text/html" << std::endl << std::endl;
    std::cout << "<html><head><title>CGI</title></head><body><h1>Hello, CGI!</h1></body></html>";
    return 0;
}