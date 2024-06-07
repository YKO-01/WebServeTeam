# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/06 12:38:17 by ayakoubi          #+#    #+#              #
#    Updated: 2024/06/05 07:52:38 by ayakoubi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# __ VAR & COMPILATION _________________________________________________________
# ==============================================================================
NAME		=	webserv
CPP			=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
INC			=	incs
OBJDIR		=	objs
SRCDIR		=	srcs
SRVDIR		=	server
CNFDIR		=	config
REQDIR		=	request
SESDIR		=	session

SRCMAIN		=	main
SRCSERV		:=	TCPServer
SRCCNFG		:=	parsing utils geter_setter
SRCREQ		:=	HTTPParser HTTPGet HTTPRequest HTTPResponse Utils
SRCSESS		:=	Session

OBJMAIN		:=	$(addprefix $(OBJDIR)/, $(addsuffix .o, $(SRCMAIN)))
SRCMAIN		:=	$(addprefix $(SRCDIR)/, $(addsuffix .cpp, $(SRCMAIN)))
OBJSERV		:=	$(addprefix $(OBJDIR)/$(SRVDIR)/, $(addsuffix .o, $(SRCSERV)))
SRCSERV		:=	$(addprefix $(SRCDIR)/$(SRVDIR)/, $(addsuffix .cpp, $(SRCSERV)))
OBJCNFG		:=	$(addprefix $(OBJDIR)/$(CNFDIR)/, $(addsuffix .o, $(SRCCNFG)))
SRCCNFG		:=	$(addprefix $(SRCDIR)/$(CNFDIR)/, $(addsuffix .cpp, $(SRCCNFG)))
OBJREQ		:=	$(addprefix $(OBJDIR)/$(REQDIR)/, $(addsuffix .o, $(SRCREQ)))
SRCREQ		:=	$(addprefix $(SRCDIR)/$(REQDIR)/, $(addsuffix .cpp, $(SRCREQ)))
OBJSESS		:=	$(addprefix $(SRCDIR)/$(SESDIR)/, $(addsuffix .o, $(SRCSESS)))
SRCSESS		:=	$(addprefix $(SRCDIR)/$(SESDIR)/, $(addsuffix .cpp, $(SRCSESS)))

# OBJSERV	:=	$(addprefix $(OBJDIR)/, $(patsubst %.cpp,%.o,$(SRCSERV)))


# __ COLOR _____________________________________________________________________
# ==============================================================================
RED = \033[1;31m
GREEN = \033[1;32m
SPLASH = \033[5m
BLUE = \033[1;34m
YELLOW = \033[1;33m
RESET = \033[0m

# __ RULES _____________________________________________________________________
# ==============================================================================
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/$(SRVDIR)
	@mkdir -p $(OBJDIR)/$(CNFDIR)
	@mkdir -p $(OBJDIR)/$(REQDIR)
	@mkdir -p $(OBJDIR)/$(SESDIR)
	@$(CPP) $(CPPFLAGS) -c $< -I $(INC) -o $@
	@echo "$(GREEN) [OK] $(RESET) $(YELLOW) compile >>>> $< $(RESET)"

all:	$(NAME)

$(NAME) :	$(OBJSERV) $(OBJMAIN) $(OBJCNFG) $(OBJREQ) $(OBJSESS)
	@$(CPP) $(CPPFLAGS) $^ -I $(INC) -o $(NAME)
	@@echo "$(GREEN) ------ Built success ------ $(RESET)"

clean:
	@echo "$(RED) REMOVED OBJ $(RESET)"
	@rm -rf $(OBJDIR)

fclean:	clean
	@echo "$(RED) REMOVED $(NAME) $(RESET)"
	@rm -rf $(NAME)

re:	fclean	all

.PHONY: all clean fclean
