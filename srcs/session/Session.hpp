/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:34:39 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/03 10:04:55 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
#define SESSION_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "ISession.hpp"
#include <ctime>

typedef std::map<std::string, std::string> mapSession;

class Session
{
	private:
		std::string generateSessionID();
	//	std::string dirName;
	public:
		Session();
		~Session();
		std::string createSession();
		std::string	getPathSession(const std::string _sessionID);
		bool validSession(const std::string _sessionID);
		std::string getSession(const std::string& _sessionID);
		void	setSession(const std::string& _sessionID, const std::string& data);
};



#endif
