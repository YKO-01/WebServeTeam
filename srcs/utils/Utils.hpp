/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 09:28:38 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/24 18:00:52 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <vector>
#include <iostream>
#include <algorithm>

# include <sys/stat.h>

typedef std::string String;
typedef std::string::iterator Iterator;
typedef std::vector<String> Vector;

namespace Utils
{
	Iterator findToken(const Iterator begin, const Iterator end, String token, bool _override = true);
	std::size_t     locateToken(const Iterator begin, const Iterator end, String token);
	std::pair<Iterator, String>     parseToken(const Iterator begin, const Iterator end, String token, bool _override = true);
	Iterator ltrimString(const Iterator begin, const Iterator end, String charset = " ");
	Iterator rtrimString(const Iterator begin, const Iterator end, String charset = " ");
	bool    isValidUriCharacters(String uri);
	bool    isValidMethod(String method);
	bool    isValidUriScheme(String scheme);
	bool    isValidUriHost(String host);
	bool    isValidUriPort(String port);
	bool    isValidUriResource(String resource);
	bool	isValidVersion(String name, String major, String minor);
	bool	isValidHeader(String header);
	bool	isnotprint(char c);
	bool    isDirectory(String path);
	bool	matchPathToRoute(String path, String route);
};

#endif
