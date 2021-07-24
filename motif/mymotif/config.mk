#==============================================================
# Config.mk -- Configurations for all subdirectory Makefiles 
# Time-stamp: <1999-09-18 15:13:40 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

# Edit the following for your installation

CC	=	gcc
CXX	=	g++
MCF	=	motif-config
UIL	=	uil
X11INC	=	/usr/include
X11LIB	=	/usr/lib
#MOTIFINC=	/opt/local/lesstif/include
#MOTIFLIB=	/opt/local/lesstif/lib
LIBS	=	 -lXm -lXt -lX11 -lXext -lxcb  

#==============================================================
# Compiler and linker flags
#CXXFLAGS=	-Wall -g -I$(X11INC) -I$(MOTIFINC)
#CFLAGS	=	-Wall -g -I$(X11INC) -I$(MOTIFINC)
#LFLAGS	=	-L$(X11LIB) -L$(MOTIFLIB)
#INCPATH	=	-L$(X11LIB) -L$(MOTIFLIB)
CXXFLAGS=	-Wall -g 
CFLAGS	=	-Wall -g
#LFLAGS	=	$(shell motif-config --libs)
#INCPATH	=	$(shell motif-config --cflags)
#LFLAGS	=	`motif-config --libs`
#INCPATH	=	`motif-config --cflags`
LFLAGS	=	`$(MCF) --libs`
INCPATH	=	`$(MCF) --cflags`
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"
