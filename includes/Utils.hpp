/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:43:28 by ael-qori          #+#    #+#             */
/*   Updated: 2024/12/20 12:23:09 by ael-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <map> 
#include <string> 
#include <iostream>
#include <stdexcept>
#include <bits/stdc++.h>

#define O_PAR "{"
#define C_PAR "}"

std::string itoa(size_t n);
std::string trim(const std::string& str);
std::vector<std::string> splitString(const std::string& input, const std::string& delimiters);

#endif