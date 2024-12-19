/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:51:19 by ael-qori          #+#    #+#             */
/*   Updated: 2024/12/19 18:39:25 by ael-qori         ###   ########.fr       */
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

ConfigParser::ConfigParser():currentServerState(HTTP), currentLocationState(PATH), currentErrorPages(ERROR_CODE), currentRedirectState(STATUS_CODE), index(0){}
                   
                    /* ---- METHODS ----*/

void    ConfigParser::parse()
{
    while (this->index < this->fileContent.size() && this->index < 4)
    {
        switch (currentServerState)
        {
            case HTTP:
                this->handleHttpState();
                break;
            case SERVER:
                this->handleServerState();
                break;
            default:
                break;
        }
    }
}

void    ConfigParser::parseFile(const char *file_path)
{
    std::ifstream file;

    file.open(file_path, std::ifstream::in);
    if (!file.is_open())
        throw std::runtime_error("Error: Filepath");
    this->fileToVector(file);
    this->checkClosedParenthesis();
    this->parse();
}

void    ConfigParser::fileToVector(std::ifstream &file)
{
    std::string     line;
    std::string     pureLine;

    while(std::getline(file, line))
        this->fileContent.push_back(line);
}

void    ConfigParser::checkClosedParenthesis()
{
    int countOpenedParenthesis = 0, countClosedParenthesis = 0, index , i, tmp;
    std::ostringstream oss;

    for(index = 0; index < this->fileContent.size(); index++)
    {
        tmp = 0;
        for (i = 0; i < this->fileContent[index].size(); ++i)
        {
            if (this->fileContent[index][i] == '{' &&( tmp = 1))
                countOpenedParenthesis++;
            if (this->fileContent[index][i] == '}' && (tmp = 1))
                countClosedParenthesis++;
        }
        this->fileContent[index] = trim(this->fileContent[index]);
        if (tmp && this->fileContent[index].size() != 1)
        {
            oss << index + 1;
            throw std::runtime_error("Syntax Error:: Line " + oss.str());
        }
    }
    if (countClosedParenthesis != countOpenedParenthesis)
        throw std::runtime_error("Syntax Error:: Missing closing parenthesis Or parenthsis not in single line");
    this->deleteEmptyLines();
}

void    ConfigParser::deleteEmptyLines()
{
    int index = -1;

    while (++index < this->fileContent.size())
        if (this->fileContent[index] == "")
            this->fileContent.erase(this->fileContent.begin() + index);
}


void    ConfigParser::handleHttpState()
{
    static bool httpStart = false;

    if (!httpStart)
    {
        std::cout << this->fileContent[this->index] << std::endl; // Temp
        if (this->fileContent[this->index++] != "http")
            throw std::runtime_error("Syntax Error::\t< http > ");
        if (this->fileContent[this->index++] != O_PAR)
            throw std::runtime_error("Syntax Error::\t< open parenthesis doesnt exist for Http> ");
        this->currentServerState = SERVER;
        httpStart = true;
    }
    else
    {
        if (this->fileContent[this->index] != C_PAR && this->fileContent.size() == this->index + 1)
                throw std::runtime_error("Syntax Error::\t< close parenthesis doesnt exist for Http> ");
        else if (this->fileContent[this->index] != C_PAR);
            this->currentServerState = SERVER;
    }
}

void    ConfigParser::handleServerState()
{
    static bool serverStart = false;
    
    if (!serverStart)
    { 
        std::cout << this->fileContent[this->index] << std::endl; // Temp
        if (this->fileContent[this->index++] != "server")
            throw std::runtime_error("Syntax Error::\t< server > ");
        if (this->fileContent[this->index++] != O_PAR)
            throw std::runtime_error("Syntax Error::\t< open parenthesis doesnt exist for Server > ");
        serverStart = true;
        // this->currentServerState = SERVER_NAME;
    }
    else
    {
        serverStart = false;
        if (this->fileContent[this->index++] != C_PAR)
            throw std::runtime_error("Syntax Error::\t< close parenthesis doesnt exist for Server > ");
        else
            this->currentServerState = HTTP;
    }       
}

                    /* ---- GET ----*/

                    /* ---- SET ----*/
