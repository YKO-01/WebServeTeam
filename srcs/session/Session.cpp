/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 21:36:44 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/03 10:05:18 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"
#include <unistd.h>
//__ Constructor _______________________________________________________________
//==============================================================================
Session::Session()
{

}

//__ Destructor ________________________________________________________________
//==============================================================================
Session::~Session()
{
}

//__ Generate Session __________________________________________________________
//==============================================================================
std::string Session::generateSessionID()
{
	srand(time(NULL) + rand());
	std::stringstream ss;
	for (int i = 0; i < 20; i++)
	{
		int r = rand() % 127;
		while(r < 32 && r > 122)
			r = rand() % 127;
		ss << char(r);
	}
	return (ss.str());
}

//-- Create Session ____________________________________________________________
//==============================================================================
std::string Session::createSession()
{
	std::string sessionID = generateSessionID();
	std::time_t now = std::time(NULL);
	setSession(sessionID, std::to_string(now));
	return (sessionID);
}

//__ Get Path Session __________________________________________________________
//==============================================================================
std::string Session::getPathSession(const std::string _sessionID)
{
	return ("sessions/" + _sessionID);
}

//__ Valid Session _____________________________________________________________
//==============================================================================
bool	Session::validSession(const std::string _sessionID)
{
	return (access(getPathSession(_sessionID).c_str(), F_OK));
}

//__ Get Session Data __________________________________________________________
//==============================================================================
std::string Session::getSession(const std::string& _sessionID)
{
	if (validSession(_sessionID) == false)
		return (std::string(""));
	std::ifstream file(getPathSession(_sessionID));
	if (file.is_open())
	{
		std::stringstream data;
		data << file;
		return (data.str());
	}
	return (std::string(""));
}

//__ Set Session Data __________________________________________________________
//==============================================================================
void	Session::setSession(const std::string& _sessionID, const std::string& data)
{
	std::cout << "session id: " << _sessionID << std::endl;
	std::ofstream file(_sessionID);
	std::cout << data << std::endl;
	file << data;
}

