#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Server.hpp"
int get_count(std::string str)
{
	int tabcount = 0;
	for (size_t l = 0; str[l] != '\0' && str[l] == '\t'; l++) 
		tabcount++;
	return tabcount;
}
void display(std::pair<std::string, std::vector<std::string> > pair)
{
	std::cout << "first:" <<  pair.first << std::endl;
	if(pair.second.size() == 0)
		return;
	else
	{
		std::cout << "secend::" << std::endl;
		for (size_t i = 0; i < pair.second.size(); i++)
		{
			std::cout << pair.second[i] << std::endl;
		}
	}
}
void split_router(std::vector<std::vector<std::string> > big_vec)
{
	std::pair<std::string, std::vector<std::string> > pair;
	std::vector<std::pair<std::string, std::vector<std::string> > > ll;
	std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > test;
	for (size_t i = 0; i < big_vec.size(); i++)
	{
		for (size_t j = 0; j < big_vec[i].size(); j++)
		{
			int tabcount = 0;
			pair.first = big_vec[i][j];
			if (big_vec[i][j].find("- router") != std::string::npos)
			{
				j++;
				tabcount = get_count(big_vec[i][j]);
				while (tabcount == get_count(big_vec[i][j]) && j < big_vec[i].size() - 1)
				{
					pair.second.push_back(big_vec[i][j]);
					j++;
				}
				j--;
				if(j == big_vec[i].size() - 1 && tabcount == get_count(big_vec[i][j]))
					pair.second.push_back(big_vec[i][j]);
			}
			ll.push_back(pair);
			pair.second.clear();
			pair.first.clear();
		}
		test.push_back(ll);
		ll.clear();
	}
	std::cout << "++-+-+-+--------++-+-+-++-+-+-+-+-+-" << std::endl;
	std::cout << "++-+-+-+--------++-+-+-++-+-+-+-+-+-" << std::endl;

	for (size_t i = 0; i < test.size(); i++)
	{
	    for (size_t j = 0; j < test[i].size(); j++)
	        display(test[i][j]);
	    std::cout << "----------------" << std::endl;
	}
}
bool isAllWhitespace(const std::string& str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) 
		if (!isspace(*it)) 
			return false;
	return true;
}
int main(int ac,char **av)
{
	std::string myText;
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
			if(get_count(myText) < 1)
			{
				std::cout << "ERROOOOOO000000r" << std::endl;
				return 0;
			}
			vct.push_back(myText);
		}
		else if(!inServerBlock)
		{
			if(get_count(myText) < 1)
			{
				std::cout << "ERROOOOOO000000r" << std::endl;
				return 0;
			}
		}
		else if(!inServerBlock)
		{
			if(get_count(myText) < 1)
			{
				std::cout << "ERROOOOOO000000r" << std::endl;
				return 0;
			}
		}
	}
	if (!vct.empty())
		big_vec.push_back(vct);
	split_router(big_vec);
}