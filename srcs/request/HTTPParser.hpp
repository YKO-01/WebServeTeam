/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:42:35 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/25 08:53:16 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include "../utils/Utils.hpp"
# include "IHTTPParser.hpp"

class HTTPParser : public IHTTPParser
{
	private:
		Method method;
		Uri uri;
		Version version;
		Map	headers;
		Status	status;

	public:
		HTTPParser(std::string request);
		virtual Status	parseStatusLine(const Iterator begin, const Iterator end);
		virtual	Method parseMethod(Iterator& begin, const Iterator end);
		virtual Uri parseUri(Iterator& begin, const Iterator end);
		virtual Version parseVersion(Iterator& begin, const Iterator end);
		virtual Header parseHeaderField(const Iterator begin, const Iterator end);
		virtual String  parseHeaderFieldName(const Iterator begin, const Iterator end);
		virtual String  parseHeaderFieldValue(const Iterator begin, const Iterator end);
		void	destroyParsedData(void);
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
		Status	getStatus(void) const;
		Method	getMethod(void) const;
		Uri	getUri(void) const;
		Version	getVersion(void) const;
		Map	getHeaders(void) const;
		~HTTPParser();
};

#endif
