# Makefile for Hex-a-hop, Copyright Oliver Pearce 2004
NAME		= Hex-a-hop
VERSION		= 1.0.0
CC		= gcc
DATA_DIR="."
CXXFLAGS		+= -Wall -W -g
GCC =g++
CXXSOURCES	= gfx.cpp hex_puzzzle.cpp i18n.cpp
#INCLUDES	= 


#############################################################

OBJS=$(CXXSOURCES:.cpp=.o)

%.o	: %.cpp
	$(GCC) $(CXXFLAGS) -D_VERSION=\"$(VERSION)\" -DDATA_DIR=\"$(DATA_DIR)\" `sdl-config --cflags`  -c -o $@ $<
	
$(NAME) : $(OBJS)
		$(GCC) $(CXXFLAGS) $(OBJS)  `sdl-config --libs` -lSDL_Pango -lm  \
		-o $(NAME)

clean :
	rm -f *~ $(OBJS) $(NAME)
