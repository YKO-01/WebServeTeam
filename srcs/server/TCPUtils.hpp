/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPUtils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:13:08 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/08 08:55:29 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPUTILS_HPP
#define TCPUTILS_HPP

#include <iostream>
#include <vector>

namespace TCPUtils
{
	std::vector<char>	storeDataInVec(char *buffer);
	int hexCharToInt(char c);
	long hexToDecimal(const std::string& hex);
	bool parseChunkedBody(std::string body);
};

#endif
