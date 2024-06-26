/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:16:52 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/10 14:54:32 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPUtils.hpp"

std::vector<char>	TCPUtils::storeDataInVec(char *buffer)
{
	std::vector<char> vec;

	vec.insert(vec.end(), buffer, buffer + strlen(buffer));
	return (vec);
}


long TCPUtils::stringToLong(const std::string& str)
{
    int octets[4];
    std::sscanf(str.c_str(), "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);
    

    return( (octets[0] << 24) | (octets[1] << 16) | (octets[2] << 8) | octets[3]);
}

int TCPUtils::hexCharToInt(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	return (0);
}

size_t TCPUtils::hexToDecimal(const std::string& hex)
{
    int result = 0;
    for (std::string::const_iterator it = hex.begin(); it != hex.end(); ++it) {
        result = result * 16 + hexCharToInt(*it);
    }
    return result;
}
/*
std::pair<size_t, std::string>	TCPUtils::parseChunkedBody(const std::string& body)
{
	size_t pos;
	size_t chunkedSize;
	std::pair<size_t, std::string> pairStr; 

	pos = body.find("\r\n");
	if (pos != std::string::npos)
	{
		chunkedSize = hexToDecimal(body.substr(0, pos));
		if (chunkedSize == 0 && body.size() > 3)
		{
			pairStr.first = -1;
			pairStr.second = body;
			return (pairStr);
		}
		else
		{
			std::string tmp = body.substr(pos + 2, body.size());
			if (chunkedSize == tmp.size() - 2)
			{
				pairStr.first = 0;
				pairStr.second.append(tmp, tmp.size() - 2);
				return (pairStr);
			}

			else if (chunkedSize > tmp.size() - 2)
			{
				pairStr.first = chunkedSize - tmp.size();
				pairStr.second.append(tmp, tmp.size());
				return (pairStr);
			}
			else
			{
				pairStr.first = chunkedSize;
				pairStr.second.append(tmp, chunkedSize);
				tmp.erase(0, chunkedSize + 2);
				parseChunkedBody(tmp);
			}
		}
	}
	pairStr.first = 0;
	pairStr.second = body;
	return (pairStr);
}
*/

std::string TCPUtils::parseChunkedBody(std::string chunk)
{
	size_t pos;
	size_t chunkedSize;
	std::string str;
	std::string tmp;

	while (chunk.size())
	{
		pos = chunk.find("\r\n");
		if (pos != std::string::npos)
		{
			tmp = chunk.substr(pos + 2, chunk.size());
			chunkedSize = hexToDecimal(chunk.substr(0, pos));
			if (chunkedSize == 0 && chunk.substr(0, pos) != "0")
				str.append(chunk, pos);
			else if (chunkedSize == 0)
				tmp.erase(tmp.begin(), tmp.end());
			else if (chunkedSize >= tmp.size())
			{
				str.append(tmp, tmp.size());
				tmp.erase(tmp.begin(), tmp.end());
			}
			else if (chunkedSize < tmp.size())
			{
				std::cout << chunkedSize << std::endl;
				str.append(tmp, 0, chunkedSize);
				tmp.erase(0, tmp.find("\r\n") + 2);
			}
			chunk = tmp;
		}
	}
	return (str);
}
