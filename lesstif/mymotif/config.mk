#==============================================================
# Config.mk -- Configurations for all subdirectory Makefiles 
# Time-stamp: <1999-09-18 15:13:40 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

# Edit the following for your installation

CC	=	g++
UIL	=	/opt/local/lesstif/bin/uil
X11INC	=	/usr/include
X11LIB	=	/usr/lib
MOTIFINC=	/opt/local/lesstif/include
MOTIFLIB=	/opt/local/lesstif/lib
LIBS	=	 -lXm -lXt -lX11 -lXext   

#==============================================================
# Compiler and linker flags

CFLAGS	=	-g -I$(X11INC) -I$(MOTIFINC)
LFLAGS	=	   -L$(X11LIB) -L$(MOTIFLIB)
