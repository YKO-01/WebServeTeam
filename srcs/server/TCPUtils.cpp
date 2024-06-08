/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:16:52 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/08 14:37:49 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPUtils.hpp"

std::vector<char>	TCPUtils::storeDataInVec(char *buffer)
{
	std::vector<char> vec;

	vec.insert(vec.end(), buffer, buffer + strlen(buffer));
	return (vec);
}

int TCPUtils::hexCharToInt(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	return (0);
}

long TCPUtils::hexToDecimal(const std::string& hex) {
    int result = 0;
    for (std::string::const_iterator it = hex.begin(); it != hex.end(); ++it) {
        result = result * 16 + hexCharToInt(*it);
    }
    return result;
}

std::pair<long, std::string>	TCPUtils::parseChunkedBody(const std::string& body)
{
	size_t pos;
	long chunkedSize;
	std::pair<long, std::string> pairStr; 

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
			if (chunkedSize == body.substr(pos + 2, body.size()))
			{

			if (chunkedSize >= body.substr(pos + 2, body.size()))
			{
				pairStr.first = chunkedSize - body;
				pairStr.second = body.substr(pos + 2, body.size());
				return (pairStr());
			}
			else
			{
				pairStr.first = chunkedSize;
				pairStr.second = pairStr.second.append(body.substr(pos, chunkedSize), chunkedSize - pos);
				parserChunkedBody(pairStr.second);
				return (pairStr);
			}
		}
	}
	pairStr.first = 0;
	pairStr.second = body;
	return (pairStr);
}

