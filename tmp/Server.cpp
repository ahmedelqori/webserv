/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:30:31 by ael-qori          #+#    #+#             */
/*   Updated: 2024/12/19 15:11:36 by ael-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void print(std::string &str)
{
    std::cout << str;
}
void printnl(std::string &str)
{
    std::cout << str << std::endl;
}

Http::Http():currentServerConfig(HTTP), currentErrorPages(ERROR_CODE), currentLocationState(PATH),currentRedirectState(STATUS_CODE){}

void Http::CheckHttpKey(std::string &line)
{
    if (line == "http")
        currentServerConfig = SERVER;
    else
    {
        error_msg = "Syntax Error:: <http> not found";
        currentServerConfig  =ERROR;
    }
}


std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) 
        return "";
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}
bool Http::CheckClosedParenthesis(std::vector<std::string> &array){
    int countOpenedParenthesis = 0, countClosedParenthesis = 0, index , i, tmp;
    
    for(index = 0; index < array.size(); index++)
    {
        tmp = 0;
        for (i = 0; i < array[index].size(); ++i)
        {
            if (array[index][i] == '{' &&( tmp = 1))
                countOpenedParenthesis++;
            if (array[index][i] == '}' && (tmp = 1))
                countClosedParenthesis++;
        }
        if (tmp && array[index].size() != 1)
            return (std::cout << "Line: " << index + 1 << std::endl , false);
    }
    return countClosedParenthesis == countOpenedParenthesis;
}

void Http::parse()
{
    std::ifstream configFile;
    std::string line;

    std::vector <std::string> configVector;
    configFile.open("file.conf");

    if (configFile.is_open())
    {
        while (std::getline(configFile, line))
        {
            line = trim(line);
            if (line.size() > 0)
                configVector.push_back(trim(line));
        }
    }
    if (!this->CheckClosedParenthesis(configVector))
    {
        std::cout << "Syntax Error:: Missing closing parenthesis Or parenthsis not in single line" << std::endl;
        exit(1);
    }   
    for(int index = 0; index < configVector.size(); index++)
    {
        switch (currentServerConfig)
        {
            case HTTP:
                this->CheckHttpKey(configVector[index]);
                break;                
            case SERVER:
                // std::cout << "One\n";
                break;
            case ERROR:
                printnl(this->error_msg);
                exit(1);
                break;
        }
        // break;
    }
}