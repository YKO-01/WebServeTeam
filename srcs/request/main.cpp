#include "HTTPRequest.hpp"

int main(int ac, char **av)
{
	Config configs;
	configs.parssConfigs((char **)av);

	std::string	request = "GET /images/users HTTP/1.1\r\n";
	HTTPRequest	r(configs.get_allInfo()[0], request);
	r.processRequest();
}
