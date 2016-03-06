#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/01/19 20:57:11 by tmielcza          #+#    #+#              #
#    Updated: 2016/03/05 20:45:33 by tmielcza         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

includedir =	include/ \
				glfw/build/include

libdir =		glfw/build/lib

libs =			glfw3

frameworks =	CoreVideo Cocoa IOKit OpenGL OpenCL Carbon

SRCDIR = src/
OBJDIR = obj/
DEPDIR = dep/
CXX = clang++
NAME = parsys
INCLUDE = $(addprefix -I,$(includedir))
LIBS = $(addprefix -L,$(libdir)) $(addprefix -l,$(libs))
FRAMEWORKS = $(addprefix -framework ,$(frameworks))
FLAGS = -Wall -Wextra -Werror -std=c++11
FLAGS += -pedantic-errors #-Weverything
#FLAGS += -O3 -march=native
FLAGS += -g -DDEBUG=ON # Debug
CXXFLAGS = $(FLAGS) $(INCLUDE)

SUBMODULES = glfw
BUILDLIBS = glfw/build/lib/libglfw3.a

vpath %.cpp $(SRCDIR)

SRC =	main.cpp \
		GPUContext.cpp \
		ParticleSystem.cpp \
		GLProgram.cpp \
		GLVAO.cpp

OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)
DEP = $(SRC:%.cpp=$(DEPDIR)/%.d)

.SECONDARY: $(OBJ)

.PHONY : all clean fclean re

all : $(DEP) $(NAME)

-include $(DEP)

$(OBJDIR)/%.o : %.cpp | $(OBJDIR)
	@printf "\e[1;32mCompiling $<...\e[0m\n"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DEPDIR)/%.d : %.cpp | $(SUBMODULES) $(DEPDIR)
	@printf "\e[1;34mGenerating $< dependencies...\n\e[0m"
	@$(CXX) $(CXXFLAGS) -MM $< -MT $(OBJDIR)/$*.o -MF $@

$(OBJDIR) :
	@mkdir -p $@

$(DEPDIR) :
	@mkdir -p $@

$(NAME) : $(BUILDLIBS) $(OBJ)
	@$(CXX) -o $@ $^ $(FLAGS) $(LIBS) $(FRAMEWORKS)
	@echo "$@ : Done!"

$(SUBMODULES) :
	@git submodule update --init --recursive

glfw/build/lib/libglfw3.a : glfw
	@cd glfw && cmake . -DGLFW_USE_CHDIR=OFF -DCMAKE_INSTALL_PREFIX=./build \
	-DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF \
	&& make install

clean :
	@rm -rf $(OBJ)
	@echo "Clean : Done."

fclean : clean
	@rm -rf $(NAME)
	@echo "Fclean : Done."

re : fclean all
