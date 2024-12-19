/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:51:19 by ael-qori          #+#    #+#             */
/*   Updated: 2024/12/19 17:15:58 by ael-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"


/* ================== Location Config ==================*/

                    /* ---- GET ----*/

bool                                LocationConfig::getDirectoryListing() const
{
    return this->directoryListing;
}

std::string                         LocationConfig::getPath() const
{
    return this->path;
}
 
std::string                         LocationConfig::getRoot() const
{
    return this->root;
}

std::map<std::string, bool>         LocationConfig::getMethods() const
{
    return this->methods;
}

std::map <std::string, std::string> LocationConfig::getRedirections() const
{
    return this->redirections;
}

                    /* ---- SET ----*/
                    

void    LocationConfig::setDirectoryListing(bool b)
{
    this->directoryListing = b;
}

void    LocationConfig::setPath(std::string &path)
{
    this->path = path;
}

void    LocationConfig::setRoot(std::string &root)
{
    this->root = root;   
}

void    LocationConfig::setMethods(std::string &key, bool value)
{
    this->methods.insert(std::make_pair(key, value));
}

void    LocationConfig::setRedirections(std::string &key, std::string &value)
{
    this->redirections.insert(std::make_pair(key, value));
}


/* ================== Server Config ==================*/

                    /* ---- GET ----*/

int                                 ServerConfig::getClientMaxBodySize() const
{
    return this->clientMaxBodySize;
}

std::string                         ServerConfig::getHost() const
{
    return this->host;
}

std::vector<std::string>            ServerConfig::getServerNames() const
{
    return this->serverNames;
}

std::vector<LocationConfig>         ServerConfig::getLocations() const
{
    return this->locations;
}

std::map<std::string, bool>         ServerConfig::getAllowMethods() const
{
    return this->allowMethods;
}

std::map<std::string, std::string>  ServerConfig::getErrorPages() const
{
    return this->errorPages;
}

                    /* ---- SET ----*/

void                                ServerConfig::setClientMaxBodySize(int size)
{
    this->clientMaxBodySize = size;
}

void                                ServerConfig::setHost(std::string &host)
{
    this->host = host;   
}

void                                ServerConfig::setServerNames(std::string &server_name)
{
    this->serverNames.push_back(server_name);
}

void                                ServerConfig::setLocations(LocationConfig &location)
{
    this->locations.push_back(location);
}

void                                ServerConfig::setAllowMethods(std::string &key, bool value)
{
    this->allowMethods.insert(std::make_pair(key, value));
}

void                                ServerConfig::setErrorPages(std::string &key, std::string &value)
{
    this->errorPages.insert(std::make_pair(key, value));   
}

/* ================== Config Parser ==================*/

                    /* ---- Class ----*/

ConfigParser::ConfigParser():currentServerState(HTTP), currentLocationState(PATH), currentErrorPages(ERROR_CODE), currentRedirectState(STATUS_CODE){}
                   
                    /* ---- METHODS ----*/

void    ConfigParser::parseFile(const char *file_path)
{
    std::ifstream file;

    file.open(file_path, std::ifstream::in);
    if (!file.is_open())
        throw std::runtime_error("Error: Filepath");
    this->fileToVector(file);
}

void    ConfigParser::fileToVector(std::ifstream &file)
{
    std::string     line;
    std::string     pureLine;

    while(std::getline(file, line))
    {
        line = trim(line);
        if (line != "")
            this->fileContent.push_back(trim(line));
    }
}
                    /* ---- GET ----*/

                    /* ---- SET ----*/
