/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:42:35 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/10 15:12:02 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include "IHTTPParser.hpp"
# include "config/Config.hpp"
# include "../utils/Utils.hpp"

class HTTPParser : public IHTTPParser
{
	private:
		Config	config;

		Method method;
		Uri uri;
		Version version;
		Map	headers;
		Status	status;

		http_encoding_t	encoding;

		std::string body;

	public:
		HTTPParser(std::string request);
		virtual Status	parseStatusLine(const Iterator begin, const Iterator end);
		virtual	Method parseMethod(Iterator& begin, const Iterator end);
		virtual Uri parseUri(Iterator& begin, const Iterator end);
		virtual Version parseVersion(Iterator& begin, const Iterator end);
		virtual	Status	parseHeaders(Iterator& begin, const Iterator end);
		virtual Header parseHeaderField(const Iterator begin, const Iterator end);
		virtual String  parseHeaderFieldName(const Iterator begin, const Iterator end);
		virtual String  parseHeaderFieldValue(const Iterator begin, const Iterator end);
		Uri	getUri(void) const;
		Map	getHeaders(void) const;
		Status	getStatus(void) const;
		Method	getMethod(void) const;
		Version	getVersion(void) const;
		Config	getConfig(void) const;
		void	destroyParsedData(void);
		void	setConfig(Config config);
		void	setBody(const std::string& body);
		String	getBody(void);
		String	getHeader(String header);
		http_encoding_t		getEncodingType(void);
		http_keepalive_t	getConnectionType(void);
		class HTTPBadMethod : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPMethodNotAllowed: public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPBadUri : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriOverflow : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriBadScheme : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriBadHost : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriBadPort : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPUriBadResource : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPBadVersion : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPVersionNotSupported : public std::exception
		{
			virtual const char	*what() const throw();
		};
		class HTTPBadHeader : public std::exception
		{
			virtual const char	*what() const throw();
		};
		~HTTPParser();
};

#endif
