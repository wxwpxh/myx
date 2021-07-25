#==============================================================
# Config.mk -- Configurations for all subdirectory Makefiles 
# Time-stamp: <1999-09-18 15:13:40 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

# Edit the following for your installation

CC	=	gcc
CXX	=	g++
UIL	=	/usr/bin/uil
X11INC	=	/usr/include
X11LIB	=	/usr/lib
MOTIFINC=	/opt/local/lesstif/include
MOTIFLIB=	/opt/local/lesstif/lib
LIBS	=	-lXm -lXaw -lXt -lXmu -lX11 -lXext -lxcb  

#==============================================================
# Compiler and linker flags
CXXFLAGS=	-Wall -g -I$(X11INC) -I$(MOTIFINC)
CFLAGS	=	-Wall -g -I$(X11INC) -I$(MOTIFINC)
LFLAGS	=	-L$(X11LIB) -L$(MOTIFLIB)
INCPATH	=	-L$(X11LIB) -L$(MOTIFLIB)

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
