#==============================================================
# Config.mk -- Configurations for all subdirectory Makefiles 
# Time-stamp: <1999-09-18 15:13:40 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

# Edit the following for your installation

CXX	=	g++
CC	=	gcc
X11INC	=	/usr/include
X11LIB	=	/usr/lib
MOTIFINC=	/usr/include
MOTIFLIB=	/usr/lib
LIBS	=	-lXm -lXaw -lXt -lXmu -lX11 -lXext -lxcb   

#==============================================================
# Compiler and linker flags
CXXFLAGS=	-Wall -g 
CFLAGS	=	-Wall -g
LFLAGS	=	$(shell motif-config --libs)
INCPATH	=	$(shell motif-config --cflags)
#LFLAGS	=	`motif-config --libs`
#INCPATH	=	`motif-config --cflags`

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
