/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 07:11:30 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/24 18:52:53 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPREQUEST_HPP
# define IHTTPREQUEST_HPP

# include "../../incs/http.h"

# include <map>
# include <vector>
# include <iostream>

typedef std::string	String;
typedef	std::string::iterator Iterator;

class IHTTPRequest
{
	public:
		virtual ~IHTTPRequest() {}
		//virtual	void parseRequest(const std::string request) = 0;
		virtual void processRequest() = 0;
};

#endif
