#include <iostream>
#include <cstdlib> // for std::getenv

int main(int ac,char **av, char **envp) {
    std::cout << "env::" << envp[0] << std::endl;
    // const char* myVar = std::getenv("MY_ENV_VARIABLE");
    // if (myVar) {
    //     std::cout << "Value of MY_ENV_VARIABLE: " << myVar << std::endl;
    // } else {
    //     std::cout << "MY_ENV_VARIABLE not set." << std::endl;
    // }
    return 0;
}
              