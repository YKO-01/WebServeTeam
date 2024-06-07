/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPMethod.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 14:23:56 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/27 13:04:40 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPMETHOD_HPP
# define IHTTPMETHOD_HPP

# include <iostream>

# include <unistd.h>
# include <dirent.h>
# include <sys/stat.h>

# include "../../incs/http.h"
# include "config/Config.hpp"

typedef std::string	String;

class IHTTPMethod
{
	public:
		IHTTPMethod() {}
		virtual Status	processResource() = 0;
		virtual Status	processFile(String resource) = 0;
		virtual Status	processDirectory(String resource) = 0;
		virtual Status	getStatus(void) const = 0;
		virtual std::string getResult(void) const = 0;
};

#endif
