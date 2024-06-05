/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPParser.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:40:30 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/05 09:40:45 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPPARSER_HPP
# define IHTTPPARSER_HPP

# include <map>
# include <iostream>

# include "../incs/http.h"

typedef std::string	String;
typedef	std::string::iterator Iterator;
typedef std::pair<std::string, std::string> Header;
typedef	std::map<String, String>	Map;

class IHTTPParser
{
	public:
		virtual ~IHTTPParser() {}
		virtual	Status parseStatusLine(const Iterator begin, const Iterator end) = 0;
		virtual Method parseMethod(Iterator& begin, const Iterator end) = 0;
		virtual Uri	parseUri(Iterator& begin, const Iterator end) = 0;
		virtual Version	parseVersion(Iterator& begin, const Iterator end) = 0;
		virtual	Status	parseHeaders(Iterator& begin, const Iterator end) = 0;
		virtual Header parseHeaderField(const Iterator begin, const Iterator end) = 0;
		virtual String	parseHeaderFieldName(const Iterator begin, const Iterator end) = 0;
		virtual String	parseHeaderFieldValue(const Iterator begin, const Iterator end) = 0;
};

#endif
