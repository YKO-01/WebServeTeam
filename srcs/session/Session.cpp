/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 21:36:44 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/05/26 15:27:31 by ayakoubi         ###   ########.fr       */
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
	for (int i = 0; i < 30; i++)
	{
		int r = rand() % 127;
		while(r < 32)
			r = rand() % 127;
		ss << char(r);
	}
	return (ss.str());
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
	if (validSession(_sessionID) == true)
		return ;
	std::ofstream file(getPathSession(_sessionID));
	file << data;
}

