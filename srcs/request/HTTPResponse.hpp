/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:28:04 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/20 11:25:14 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "IHTTPResponse.hpp"

class HTTPResponse : public IHTTPResponse
{
	private:
		std::string	body;
		Status status_code;

	public:
		HTTPResponse();
		virtual void push_body(std::string body);
		virtual std::string generate(void);
		virtual	void setStatusCode(const Status& code);
		virtual	Status	getStatusCode(void) const;
		~HTTPResponse();
};

#endif
