/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:33:35 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/05/03 12:20:17 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserve.hpp"
#include "build_server.hpp"

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
	/*std::string myText;
    int index = 0;
    std::ifstream MyReadFile(av[1]);
    std::vector<std::string > vct;
    std::vector<std::vector<std::string> > big_vec;
    size_t pos;
    std::pair<std::string, std::string> pair;
    bool inServerBlock = false;
    while (getline (MyReadFile, myText))
    {
        if(myText.empty() || isAllWhitespace(myText))
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
    Server servers;
    std::vector<Server> all_info;
    all_info = insert_data_to_server(server_router_info, servers);*/
	TCPServer server;
	server.fillVectorConfigs();
	if (!server.initSocket())
		return (1);
	server.runServer();
	return (0);	
}
