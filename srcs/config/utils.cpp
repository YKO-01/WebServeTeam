/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 14:32:31 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/05/25 10:25:05 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

void ft_exit(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}

int has_space_in_front(const std::string& str) 
{
    if(str[0] == ' ')
        return 1;
    return 0;
}

std::string trim(const std::string& str) 
{
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(str[start])) 
        start++;

    while (end > start && std::isspace(str[end - 1])) 
        end--;

    return str.substr(start, end - start);
}
std::string trim_tabs(const std::string& str)
{
    size_t start = 0;
    size_t end = str.length();

    while (start < end && str[start] == '\t') 
        start++;

    return str.substr(start, end - start);
}

int get_count(std::string str,char token,int all)
{
    int tabcount = 0;
    if(all == 0)
        for (size_t l = 0; str[l] != '\0' && str[l] == '\t'; l++) 
            tabcount++;
    else if(all == 1)
    {
        for (size_t l = 0; str[l] != '\0'; l++) 
        if(str[l] == token)
            tabcount++;
    }
    else
    {
        bool insideQuotes = false;
        for (size_t l = 0; str[l] != '\0'; l++)
        {
            if (str[l] == '"') 
                insideQuotes = !insideQuotes;
            else if (str[l] == ':' && !insideQuotes) 
                tabcount++;
        }
    }
    return tabcount;
}

bool isAllWhitespace(const std::string& str) 
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) 
        if (!isspace(*it)) 
            return false;
    return true;
}

std::vector<std::string> split_stream(std::string str,char token)
{
    std::istringstream iss(str);
    std::vector<std::string> results;
    std::string temp;
    while (std::getline(iss, temp, token)) 
        if (!temp.empty()) 
            results.push_back(trim(temp));
    return results;
}
