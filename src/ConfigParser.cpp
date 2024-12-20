/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:51:19 by ael-qori          #+#    #+#             */
/*   Updated: 2024/12/20 14:40:24 by ael-qori         ###   ########.fr       */
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

int                                 ServerConfig::getPort() const
{
    return this->port;   
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

void                                ServerConfig::setPort(int port)
{
    this->port = port;   
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

ConfigParser::ConfigParser():currentServerState(HTTP), currentLocationState(PATH), currentErrorPages(ERROR), currentRedirectState(STATUS_CODE), index(0),current(0){}
                   
                    /* ---- METHODS ----*/

void    ConfigParser::parse()
{
    while (this->index < this->fileContent.size() && currentServerState != DONE)
    {
        switch (currentServerState)
        {
            case HTTP:              this->handleHttpState();        break;
            case SERVER:            this->handleServerState();      break;
            case HOST_PORT:         this->handleHostPortState();    break;
            case SERVER_NAME:       this->handleServerNameState();  break;
            case ERROR_PAGES:       this->parseErrorPages();        break;
            default:                                                break;
        }
    }
}

void    ConfigParser::parseFile(const char *file_path)
{
    std::ifstream file;

    file.open(file_path, std::ifstream::in);
    if (!file.is_open()) throw std::runtime_error("Error: Filepath");
    this->fileToVector(file);
    this->checkClosedParenthesis();
    this->parse();
}

void    ConfigParser::fileToVector(std::ifstream &file)
{
    std::string     line;
    std::string     pureLine;
    while(std::getline(file, line)) this->fileContent.push_back(line);
}

void    ConfigParser::checkClosedParenthesis()
{
    int countOpenedParenthesis = 0, countClosedParenthesis = 0, index , i, tmp;

    for(index = 0; index < this->fileContent.size(); index++)
    {
        tmp = 0;
        for (i = 0; i < this->fileContent[index].size(); ++i)
        {
            if (this->fileContent[index][i] == '{' &&( tmp = 1)) countOpenedParenthesis++;
            if (this->fileContent[index][i] == '}' && (tmp = 1)) countClosedParenthesis++;
        }
        this->fileContent[index] = trim(this->fileContent[index]);
        if (tmp && this->fileContent[index].size() != 1) throw std::runtime_error("Syntax Error:: Line " + itoa(index + 1));
    }
    if (countClosedParenthesis != countOpenedParenthesis) throw std::runtime_error("Syntax Error:: Missing closing parenthesis Or parenthesis not in single line");
    this->deleteEmptyLines();
}

void    ConfigParser::deleteEmptyLines()
{
    int index = -1;

    while (++index < this->fileContent.size()) if (this->fileContent[index] == "")  this->fileContent.erase(this->fileContent.begin() + index);
}


void    ConfigParser::handleHttpState()
{
    static bool httpStart = false;

    if (!httpStart)
    {
        if (this->fileContent[this->index++] != "http") throw std::runtime_error("Syntax Error::\t< http > ");
        if (this->fileContent[this->index++] != O_PAR) throw std::runtime_error("Syntax Error::\t< open parenthesis doesnt exist for Http> ");
        this->currentServerState = SERVER;
        httpStart = true;
    }
    else
    {
        if (this->fileContent[++this->index] != C_PAR && this->fileContent.size() == this->index + 1) throw std::runtime_error("Syntax Error::\t< close parenthesis doesnt exist for Http> ");
        else if (this->fileContent[this->index] != C_PAR) this->currentServerState = SERVER;
        else  this->currentServerState = DONE;
    }
}

void    ConfigParser::handleServerState()
{
    static bool serverStart = false;

    this->servers.push_back(ServerConfig());
    if (!serverStart)
    { 
        if (this->fileContent[this->index++] != "server") throw std::runtime_error("Syntax Error::\t< server > ");
        if (this->fileContent[this->index++] != O_PAR) throw std::runtime_error("Syntax Error::\t< open parenthesis doesnt exist for Server > ");
        (serverStart = true, this->currentServerState = HOST_PORT);
    }
    else
    {
        serverStart = false;
        if (this->fileContent[this->index] != C_PAR) throw std::runtime_error("Syntax Error::\t< close parenthesis doesnt exist for Server > ");
        else (this->current++ ,this->currentServerState = HTTP);
    }       
}

void    ConfigParser::handleHostPortState()
{
    std::vector<std::string> hostPort;
    
    if (!this->servers[this->current].getHost().empty()) throw std::runtime_error("Syntax Error::\t< Duplicated ServerName > ");
    hostPort = splitString(this->fileContent[this->index++], " \t\n\r\f\v");
    if (hostPort[0] != "host_port" || hostPort.size() != 2) throw std::runtime_error("Syntax Error::\t< host_port in server " + itoa(this->current) + " >");
    hostPort = splitString(hostPort[1], ":");
    if (hostPort.size() != 2) throw std::runtime_error("Syntax Error::\t< host_port in server " + itoa(this->current) + " >");
    this->servers[this->current].setHost(hostPort[0]);
    this->servers[this->current].setPort(atoi(hostPort[1].c_str()));
    this->currentServerState = SERVER_NAME;
}

void    ConfigParser::handleServerNameState()
{
    std::vector<std::string> server_names;
    int                      i = 0;

    if (!this->servers[this->current].getServerNames().empty()) throw std::runtime_error("Syntax Error::\t< Duplicated ServerName > ");
    server_names = splitString(this->fileContent[this->index++], " \t\n\r\f\v");
    if (server_names[0] != "server_names" || server_names.size() < 2) throw std::runtime_error("Syntax Error::\t< server_names in server " + itoa(this->current) + " >");
    while (++i < server_names.size()) this->servers[this->current].setServerNames(server_names[i]);
    this->currentServerState = ERROR_PAGES;
}


void    ConfigParser::parseErrorPages()
{
    while(this->index < this->fileContent.size() && currentErrorPages != DONE_ERROR_PAGES)
    {
        switch (currentErrorPages)
        {
            case ERROR:         this->handleErrorPagesState(); break;
            case ERROR_FILE:    this->handleErrorFileState(); break;
            default: break;
        }
    }
    currentErrorPages = ERROR;
}


void    ConfigParser::handleErrorPagesState()
{
    static bool errorPagesState = false;

    if (!errorPagesState)
    {
        if (this->fileContent[this->index++] != "errors_pages") throw std::runtime_error("Syntax Error::\t< ErrorPages > ");
        if (this->fileContent[this->index++] != O_PAR) throw std::runtime_error("Syntax Error::\t< open parenthesis doesnt exist for Error Pages > ");
        (this->currentErrorPages = ERROR_FILE ,errorPagesState = true);
    }
    else
    {
        if (this->fileContent[this->index] != C_PAR) this->currentErrorPages = ERROR_FILE;
        else (this->index++, this->currentErrorPages = DONE_ERROR_PAGES, this->currentServerState = SERVER, errorPagesState = false);
    }
}

void    ConfigParser::handleErrorFileState()
{
    std::vector<std::string> error_page;

    error_page = splitString(this->fileContent[this->index++], " \t\n\r\f\v");
    if (error_page[0] != "error" || error_page.size() != 3) throw std::runtime_error("Syntax Error::\t< error file in server " + itoa(this->current) + " >");
    this->servers[this->current].setErrorPages(error_page[1], error_page[2]);
    this->currentErrorPages = ERROR;
}

                    /* ---- GET ----*/

                    /* ---- SET ----*/
