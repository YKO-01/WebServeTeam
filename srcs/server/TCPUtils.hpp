/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPUtils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:13:08 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/09 10:17:06 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPUTILS_HPP
#define TCPUTILS_HPP

#include <iostream>
#include <sstream>
#include <vector>

namespace TCPUtils
{
	std::vector<char>	storeDataInVec(char *buffer);
	long stringToLong(const std::string& str);
	int hexCharToInt(char c);
	size_t hexToDecimal(const std::string& hex);
	std::pair<size_t, std::string> parseChunkedBody(const std::string& body);
};

#endif
