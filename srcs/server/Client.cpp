/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:20:35 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/10 12:39:06 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    isChunked = 0;
    readNum = 0;
    sendNum = 0;
    isHeader = false;
    isBody = false;
}

Client::~Client()
{
    
}

std::string Client::getRequest() const
{
    return request;
}
std::string Client::getRestBody() const
{
    return restBody;
}

size_t Client::getReadNum() const
{
    return readNum;
}

size_t Client::getSendNum() const
{
    return sendNum;
}

int Client::getIsChunked() const
{
    return isChunked;
}

HTTPParser* Client::getHTTPParser() const
{
    return (httpParser);
}

void Client::setRequest(const std::string& request)
{
    this->request = request;
}

void Client::setReadNum(const size_t& readNum)
{
    this->readNum = readNum;
}

void  Client::setSendNum(const size_t& sendNum)
{
    this->sendNum = sendNum;
}

void  Client::setRestBody(const std::string& restBody)
{
    this->restBody = restBody;
}

void Client::setIsChunked(const int& isChunked)
{
    this->isChunked = isChunked;
}

void Client::setHTTPParser(HTTPParser *httpParser)
{
    this->httpParser = httpParser;
}
