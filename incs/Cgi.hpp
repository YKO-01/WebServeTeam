/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasbaou <hkasbaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:29:56 by hkasbaou          #+#    #+#             */
/*   Updated: 2024/05/31 16:02:58 by hkasbaou         ###   ########.fr       */
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
        static std::string check_extension_file(const std::string &file);
        static std::string split_equal(const std::string &str);
        static void exec_cpp(const std::string &path, char **env);
        static std::string exec_cgi();
};