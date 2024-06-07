/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 10:00:28 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/05 07:54:40 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(Config config, std::string request) : config(config), state(HTTP_WAITING), response(new HTTPResponse())
{
	parseRequest(request);
}

void	HTTPRequest::parseRequest(const std::string& request)
{
	HTTPParser *parser = new HTTPParser(request);
	method = parser->getMethod();
	uri = parser->getUri();
	version = parser->getVersion();
	headers = parser->getHeaders();

	response->setStatusCode(parser->getStatus());
	delete parser;
}

void	HTTPRequest::processRequest()
{
	IHTTPMethod	*builder;

	if (!getMatchedLocation(uri.resource))
		return ;
	if (route.get_redirect().length())
	{
	}
	builder = buildRequest();
	if (!builder)
		return ;
	builder->processResource();
}

IHTTPMethod*	HTTPRequest::buildRequest()
{
	IHTTPMethod	*builder;

	switch (method)
	{
		case GET:
			builder = new HTTPGet(route, uri.resource);
			break ;
		case POST:
			builder = NULL;
			break ;
		case DELETE:
			builder = NULL;
			break ;
		default:
			builder = NULL;
	}
	return (builder);
}

bool	HTTPRequest::getMatchedLocation(const String& resource)
{
	(void) resource;
	bool	res;
	std::vector<Route>	routes;

	res = false;
	routes = config.get_routes();
	for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); it++)
	{
		if (Utils::matchPathToRoute(uri.resource, it->get_path()))
		{
			if (route.get_path().length() < it->get_path().length())
				route = *it;
			res = true;
		}
	}
	return (res);
}

HTTPRequest::~HTTPRequest()
{
	delete response;
}
