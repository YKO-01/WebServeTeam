# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayakoubi <ayakoubi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/06 12:38:17 by ayakoubi          #+#    #+#              #
#    Updated: 2024/05/06 11:56:25 by ayakoubi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# __ VAR & COMPILATION _________________________________________________________
# ==============================================================================
NAME		=	webserve
CPP			=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98
INC			=	incs
OBJDIR		=	objs
SRCDIR		=	srcs
BUILDDIR	=	build_server
PARSSDIR	=	parsingFile

SRCMAIN		=	main
SRCBUILD	:=	TCPServer initSocket
SRCPARSS	:=	parsing utils geter_setter

OBJMAIN		:=	$(addprefix $(OBJDIR)/, $(addsuffix .o, $(SRCMAIN)))
SRCMAIN		:=	$(addprefix $(SRCDIR)/, $(addsuffix .cpp, $(SRCMAIN)))
OBJBUILD	:=	$(addprefix $(OBJDIR)/$(BUILDDIR)/, $(addsuffix .o, $(SRCBUILD)))
SRCBUILD	:=	$(addprefix $(SRCDIR)/$(BUILDDIR)/, $(addsuffix .cpp, $(SRCBUILD)))
OBJPARSS	:=	$(addprefix $(OBJDIR)/$(PARSSDIR)/, $(addsuffix .o, $(SRCPARSS)))
SRCPARSS	:=	$(addprefix $(SRCDIR)/$(PARSSDIR)/, $(addsuffix .cpp, $(SRCPARSS)))

# OBJBUILD	:=	$(addprefix $(OBJDIR)/, $(patsubst %.cpp,%.o,$(SRCBUILD)))


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
	@mkdir -p $(OBJDIR)/$(BUILDDIR)
	@mkdir -p $(OBJDIR)/$(PARSSDIR)
	@$(CPP) $(CPPFLAGS) -c $< -I $(INC) -o $@
	@echo "$(GREEN) [OK] $(RESET) $(YELLOW) compile >>>> $< $(RESET)"

all:	$(NAME)

$(NAME) :	$(OBJBUILD) $(OBJMAIN) $(OBJPARSS)
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
