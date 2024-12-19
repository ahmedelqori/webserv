/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:31:46 by ael-qori          #+#    #+#             */
/*   Updated: 2024/12/19 15:05:20 by ael-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <map>

#include "Global.hpp"

class Location
{
    private:
        std::string path;
        std::string root;
        bool directoryListing;
        std::map <std::string, bool> methods;
        std::map <std::string, std::string> redirections;    
};

class Server
{
    private:
        std::string host;
        std::string serverNames;
        const int clientMaxBodySize;
        std::vector<Location> locations;
        std::map<std::string, bool> allowMethods;
        std::map<std::string, std::string> errorPages;
    public:
        std::string getHost() const;
        void setHost(std::string &str);

        std::string getServerName() const;
        void setServerName(std::string &str);
};

class Http
{
    private:
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

        enum  ServerConfigState
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
        std::string error_msg;
        ErrorPagesState currentErrorPages;
        LocationsState currentLocationState;
        ServerConfigState currentServerConfig;
        RedirectionsState currentRedirectState;
        Http();
        std::vector <Server> servers;
        void parse();
        void handleLocations();


        bool CheckClosedParenthesis(std::vector<std::string> &array);
        void CheckHttpKey(std::string &line);
};