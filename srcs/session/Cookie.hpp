/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 10:58:46 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/05/31 15:34:58 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIES_HPP
#define COOKIES_HPP

#include <iostream>

class Cookie
{
	private:
		std::string name;
		std::string value;
		int	maxTime;
	public:
		Cookie(std::string name, std::string value, int maxTime);
		~Cookie();
		const std::string Set() const;

};

#endif
