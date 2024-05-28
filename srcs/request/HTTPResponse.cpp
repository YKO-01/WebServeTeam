/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:20:12 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/20 11:25:44 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{
}

void	HTTPResponse::push_body(std::string body)
{
	this->body = body;
}

std::string	HTTPResponse::generate(void)
{
	return ("<html><body>No Contnet!</body></html>");
}

void	HTTPResponse::setStatusCode(const Status& code)
{
	this->status_code = code;
}

Status	HTTPResponse::getStatusCode(void) const
{
	return (status_code);
}

HTTPResponse::~HTTPResponse()
{
}
