/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:07:10 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/03/26 14:07:11 by hkasbaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <map>
class Route {
private:
    

public:
    std::string path;
    std::vector<std::string> methods;
    std::string directory;
    std::string redirect;
    std::string file; 
    bool directory_listing;
    
    void clear_route();
    // Add getters and setters as needed
};