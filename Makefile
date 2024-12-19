# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/19 15:51:13 by ael-qori          #+#    #+#              #
#    Updated: 2024/12/19 16:39:02 by ael-qori         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = c++
CFLAGS = -std=c++98 #-Wall -Wextra -Werror  #-fsanitize=address
RM = rm -rf

SRC = ./src/WebServ.cpp ./src/ConfigParser.cpp
LIB =  ./includes/ConfigParser.hpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(LIB)
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all