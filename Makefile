# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/15 11:35:49 by bcosters          #+#    #+#              #
#    Updated: 2021/09/20 13:27:41 by bcosters         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	~ Pipex ~	#

# -*- Definitions of variables -*-

NAME	= pipex

OBJDR	= obj/
LIBFTDR	= libft/

HEADER	= pipex.h

MAINSRC	= pipex.c
SRCS	= ${shell find src -type f -name "*.c"}
OBJS	= ${SRCS:src/%.c=obj/%.o}

CC		= gcc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror
LFLAGS	= -L$(LIBFTDR) -lft #-L42_memleak_check/ -lmlc

# -*- The Rules -*-

#	Implicit rules

%.o: %.c
			@$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.c
			@printf "#"
			@$(CC) $(CFLAGS) -c $< -o $@

#	Active rules

all:		$(NAME)

# Lesson in not making your rule names the same as varaible :facepalm:
lib:
			@$(MAKE) --silent -C $(LIBFTDR)

$(NAME):	lib progress $(OBJDR) $(OBJS) $(HEADER)
			@$(CC) $(CFLAGS) $(MAINSRC) ${wildcard src/*.c} $(LFLAGS) -o $(NAME)
			@printf "]\n"
			@echo "FINISHED: Compiling MANDATORY $(NAME)"

bonus:		lib progress $(OBJDR) $(OBJS) $(HEADER)
			@$(CC) $(CFLAGS) $(MAINSRC) ${wildcard src/*.c} -L$(LIBFTDR) -lft -o $(NAME)
			@printf "]\n"
			@echo "FINISHED: Compiling BONUS $(NAME)"

progress:
			@printf "Compiling executable $(NAME)\n["

$(OBJDR):
			@mkdir -p $(OBJDR)

#	Cleaning rules

clean:
			@echo Removing all object files
			@rm -rf ${OBJDR}
			@cd $(LIBFTDR) && make $@

fclean:		clean
			@echo "Removing $(NAME)"
			@${RM} ${NAME} ${CHECK}
			@echo "Removing libft library"
			@cd $(LIBFTDR) && make $@

re:			fclean all

.PHONY:		all clean fclean re
