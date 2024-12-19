/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:31:14 by ael-qori          #+#    #+#             */
/*   Updated: 2024/12/19 17:26:52 by ael-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"

int main(int ac, char **av, char **env)
{
    ConfigParser    configParser;
    
    try {
        if (ac != 2) {
            throw std::runtime_error("Error: Incorrect number of arguments. Expected 1 argument.");
        }
        configParser.parseFile(av[1]);
    } catch (const std::exception& e) {
        std::cerr << std::endl <<e.what() << std::endl << std::endl;
        return 1; 
    }
}