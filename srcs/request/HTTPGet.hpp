/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPGet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:24:44 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/28 12:11:49 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGET_HPP
# define HTTPGET_HPP

# include <fstream>
# include <sstream>

# include "IHTTPMethod.hpp"

# include "../utils/Utils.hpp"

class HTTPGet : public IHTTPMethod
{
	private:
		Route target;
		Status status;
		std::string result;
		std::string location;
		std::string	resource;
		std::string	absolute_resource;

	public:
		HTTPGet(Route route, std::string resource);
		virtual Status	processResource();

		virtual Status	processFile(std::string resource);
		virtual Status	processDirectory(std::string resource);
		virtual	std::string	getResult(void) const;
		virtual	Status	getStatus(void) const;

		std::string	readFile(std::string resource);
		std::string getAutoIndex(std::string resource);
		std::string	getHyperlink(std::string& link);
		std::string	getIndexEntry(std::string directory, std::string resource);

		~HTTPGet();
};

#endif
