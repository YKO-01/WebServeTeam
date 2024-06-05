/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtility.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:37:32 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/03 09:29:59 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseUtility.hpp"

std::string	ResponseUtility::getCurrentTime()
{
	std::time_t currentTime = std::time(nullptr);

    std::tm* timeInfo = std::gmtime(&currentTime);
    if (timeInfo == nullptr) {
        std::cerr << "Error getting current time.\n";
        return "";
    }

    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);

    return std::string(buffer);
}

std::string	ResponseUtility::translateStatus(Status status)
{
	std::string	reason;

	switch (status)
	{
		case HTTP_CONTINUE:
			reason = "Continue";
			break ;
		case HTTP_SWITCHING_PROTOCOLS:
			reason = "Switching Protocols";
			break ;
		case HTTP_OK:
			reason = "OK";
			break ;
		case HTTP_CREATED:
			reason = "Created";
			break ;
		case HTTP_ACCEPTED:
			reason = "Accepted";
			break ;
		case HTTP_NOT_AUTHORITATIVE:
			reason = "Non-Authoritative Information";
			break ;
		case HTTP_NO_CONTENT"
			reason = "No Content";
			break ;
		case HTTP_RESET_CONTENT:
			reason = "Reset Content";
			break ;
		case HTTP_PARTIAL_CONTENT:
			reason = "Partial Content";
			break ;
		case HTTP_MULTIPLE_CHOICES:
			reason = "Multiple Choices";
			break ;
		case HTTP_MOVED_PERMANENTLY:
			reason = "Moved Permanently";
			break ;
		case HTTP_FOUND:
			reason = "Found";
			break ;
		case HTTP_SEE_OTHER:
			reason = "See Other";
			break ;
		default:
			reason = "Not Recognized";
	}
}
