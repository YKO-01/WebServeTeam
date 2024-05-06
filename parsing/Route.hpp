/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:07:10 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/05/03 19:58:14 by hkasbaou         ###   ########.fr       */
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
    std::string default_file;
    std::string cgi_bin;
    std::string cgi_extension;
    bool directory_listing;

    //geters
    std::string get_path();
    std::vector<std::string> get_methods();
    std::string get_directory();
    std::string get_redirect();
    std::string get_file();
    std::string get_default_file();
    std::string get_cgi_bin();
    std::string get_cgi_extension();
    bool get_directory_listing();
    //setters
    
    void clear_route();
    // Add getters and setters as needed
};