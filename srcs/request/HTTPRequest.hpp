/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:32:55 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/27 09:52:25 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "IHTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPParser.hpp"
# include "HTTPGet.hpp"

class HTTPRequest : public IHTTPRequest
{
	private:
		Config config;
		Route route;

		Method method;
		Uri uri;
		Version version;
		State state;
		std::map<String, String>	headers;
		std::vector<uint16_t>	body;

		HTTPResponse	*response;
		void	parseRequest(const std::string& request);

	public:
		HTTPRequest(Config config, std::string request);
		virtual void processRequest();
		IHTTPMethod*	buildRequest();
		bool	getMatchedLocation(const String& resource);
		void	printRequest(void);
		~HTTPRequest();
};

#endif
