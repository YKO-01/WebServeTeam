/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISession.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 21:03:36 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/03 09:53:55 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISESSION_HPP
#define ISESSION_HPP

#include <iostream>

class ISession
{
	public:
		virtual ~ISession();
		virtual std::string generateSessionID() = 0;
		virtual	std::string createSesssion() = 0;
		virtual std::string getPathSession(const std::string _sessionID) = 0;
		virtual bool validSession(const std::string _sessionID) = 0;
		virtual std::string getSession(const std::string& _sessionID) = 0;
		virtual void setSession(const std::string& _sessionID, const std::string& data) = 0;
};

#endif

