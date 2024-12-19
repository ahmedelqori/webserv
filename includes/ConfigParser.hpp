/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:51:17 by ael-qori          #+#    #+#             */
/*   Updated: 2024/12/19 16:40:21 by ael-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>
#include <bits/stdc++.h>
#include <map> 
class LocationConfig
{
    private:
        bool                                directoryListing;
        std::string                         path;
        std::string                         root;
        std::map <std::string, bool>        methods;
        std::map <std::string, std::string> redirections;
    public:
        bool                                getDirectoryListing() const;
        std::string                         getPath() const; 
        std::string                         getRoot() const;
        std::map<std::string, bool>         getMethods() const;
        std::map <std::string, std::string> getRedirections() const;

        void                                setDirectoryListing(bool b);
        void                                setPath(std::string &path); 
        void                                setRoot(std::string &root);
        void                                setMethods(std::string &key, bool value);
        void                                setRedirections(std::string &key, std::string &value);
        
};

class ServerConfig
{
    private:
        int                                 clientMaxBodySize;
        std::string                         host;
        std::vector<std::string>            serverNames;
        std::vector<LocationConfig>         locations;
        std::map<std::string, bool>         allowMethods;
        std::map<std::string, std::string>  errorPages;
    public:
        int                                 getClientMaxBodySize() const;
        std::string                         getHost() const;
        std::vector<std::string>            getServerNames() const;
        std::vector<LocationConfig>         getLocations() const;
        std::map<std::string, bool>         getAllowMethods() const;
        std::map<std::string, std::string>  getErrorPages() const;

        void                                setClientMaxBodySize(int size);
        void                                setHost(std::string &host);
        void                                setServerNames(std::string &server_name);
        void                                setLocations(LocationConfig &location);
        void                                setAllowMethods(std::string &key, bool value);
        void                                setErrorPages(std::string &key, std::string &value);
    
    
};

class ConfigPraser
{
    private:
        std::vector <ServerConfig> servers;
        enum  ErrorPagesState
        {
            ERROR_CODE,
            FILE,
        };

        enum  RedirectionsState
        {
            STATUS_CODE,
            REDIRECTION_FILE  
        };

        enum  LocationsState
        {
            PATH,
            METHODS,
            ROOT,
            DIRECTORY_LISING,
            REDIRECTIONS
            
        };

        enum  ServerState
        {
            HTTP,
            SERVER,
            SERVER_NAME,
            HOST_PORT,
            ERROR_PAGES,
            ALLOW_METHODS,
            CLIENT_MAX_BODY_SIZE,
            LOCATIONS,
            ERROR
        };
    public:
        ServerState         currentServerState;
        ErrorPagesState     currentErrorPages;
        LocationsState      currentLocationState;
        RedirectionsState   currentRedirectState;
        
        ConfigPraser();
    
        void    parseFile(std::string &file_path);

};

#endif