/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPGet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 18:16:21 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/05/28 12:02:38 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPGet.hpp"

HTTPGet::HTTPGet(Route route, std::string resource) : target(route)
{
	this->resource = resource.substr(target.get_path().length(), resource.length());
	this->absolute_resource = target.get_directory() + this->resource;
}

Status HTTPGet::processResource()
{
	if (Utils::isDirectory(absolute_resource))
		status = processDirectory(resource);
	else
		status = processFile(absolute_resource);
	std::cout << " ===================== Data ======================" << std::endl;
	std::cout << " location : " << location << std::endl;
	std::cout << " status : " << status << std::endl;
	std::cout << result;
	std::cout << " =================================================" << std::endl;
	return (status);
}

Status	HTTPGet::processFile(String resource)
{
	if (access(resource.c_str(), F_OK) < 0)
		return (HTTP_NOT_FOUND);
	if (access(resource.c_str(), R_OK) < 0)
		return (HTTP_FORBIDDEN);
	if (target.get_useCGI() && 0)
		;
	else
		result = readFile(resource);
	return (HTTP_CONTINUE);
}

String	HTTPGet::readFile(String resource)
{
	std::ifstream file(resource);
	if (!file.is_open())
		return ("");
	std::stringstream	buffer;
	buffer << file.rdbuf();
	return (buffer.str());
}

Status	HTTPGet::processDirectory(String resource)
{
	if (resource[resource.length() - 1] != '/')
	{
		location = target.get_path() + resource + "/";
		return (HTTP_MOVED_PERMANENTLY);
	}
	if (!target.get_default_file().empty() && resource.find_first_not_of("/") == String::npos)
	{
		resource = absolute_resource + target.get_default_file();
		if (!access(resource.c_str(), F_OK))
			return (processFile(resource));
	}
	if (target.get_directory_listing())
	{
		result = getAutoIndex(absolute_resource);
		return (HTTP_CONTINUE);
	}
	return (HTTP_FORBIDDEN);
}

String	HTTPGet::getAutoIndex(String resource)
{
	DIR	*directory;
	struct dirent*	dr;
	String	index;

	directory = opendir(resource.c_str());
	if (!directory)
		;
	index ="<html>\r\n";
	index += "<head><title>Index of " + target.get_path() + this->resource + "</title></head>\r\n";
	index += "<body>\r\n";
	index += "<h1>Index of " + target.get_path() + this->resource + "</h1><hr><pre>";
	index += "<a href=\"../\">../</a>\r\n";
	dr = readdir(directory);
	while (dr)
	{
		if (std::strcmp(dr->d_name, ".") != 0 && strcmp(dr->d_name, "..") != 0)
			index += getIndexEntry(resource, dr->d_name);
		dr = readdir(directory);
	}
	index += "</pre><hr></body>\r\n";
	index += "</html>\r\n";
	return (index);
}

String	HTTPGet::getHyperlink(String& file)
{
	String a;

	a = "<a href=\"" + file + "\">";
	file = (file.length() < 50 ? file : file.substr(0, 46) + "..>");
	a += file;
	a += "</a>";
	return (a);
}

String	HTTPGet::getIndexEntry(std::string directory, std::string resource)
{
	String entry;
	struct stat fileStat;
	struct tm* timeinfo;
	char	buffer[20];

	if (stat((directory + resource).c_str(), &fileStat) < 0)
		return (entry);
	timeinfo = localtime(&fileStat.st_mtime);
	if (S_ISDIR(fileStat.st_mode))
	{
		resource += "/";
		entry = getHyperlink(resource) + String(50 - resource.length(), ' ');
		strftime(buffer, 20, "%d-%b-%Y %H:%M", timeinfo);
		entry += buffer + String(20, ' ') + "-";
	}
	else
	{
		entry = getHyperlink(resource) + String(50 - resource.length(), ' ');
		strftime(buffer, 20, "%d-%b-%Y %H:%M", timeinfo);
		entry += buffer + String(20, ' ') + std::to_string(fileStat.st_size);
	}
	entry += "\r\n";
	return (entry);
}

String	HTTPGet::getResult(void) const
{
	return (result);
}

Status	HTTPGet::getStatus(void) const
{
	return (status);
}
