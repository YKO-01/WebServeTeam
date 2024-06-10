/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:29:56 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/06/06 13:16:57 by hkasbaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>

class   CGI
{
    private:
        std::map<std::string, std::string> _env;
        
    public:
        CGI(std::map<std::string, std::string> _env);
        std::string check_extension_file(const std::string &file);
        // std::string split_equal(const std::string &str);
        void exec_cpp(const std::string &path, char **env);
        std::string exec_cgi();
        char** set_env();
};