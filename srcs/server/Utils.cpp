/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayakoubi <ayakoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:16:52 by ayakoubi          #+#    #+#             */
/*   Updated: 2024/06/06 08:23:16 by ayakoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::vector<char>	Utils::storeDataInVec(char *buffer)
{
	std::vector<char> vec;

	vec.insert(vec.end(), buffer, buffer + strlen(buffer));
	return (vec);
}
