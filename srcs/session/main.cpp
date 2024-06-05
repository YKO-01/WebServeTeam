/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:37:11 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/03 10:03:04 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"
#include <unistd.h>

int main(int ac, char **av, char **env)
{
	Session _session;
	if (ac != 2)
		return (0);	
	chdir("sessions");

	std::string data = av[1];

	std::string sessionID = _session.createSession();
	std::cout << sessionID << std::endl;
	_session.setSession(sessionID, data);
	std::cout << _session.getSession(sessionID) << std::endl;
	return (0);
}
