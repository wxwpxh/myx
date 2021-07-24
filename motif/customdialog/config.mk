#==============================================================
# Config.mk -- Configurations for all subdirectory Makefiles 
# Time-stamp: <1999-09-18 15:13:40 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

# Edit the following for your installation

CC	=	g++
X11INC	=	/usr/include
X11LIB	=	/usr/lib
MOTIFINC=	/usr/include
MOTIFLIB=	/usr/lib
LIBS	=	 -lXm -lXt -lX11 -lXext   

#==============================================================
# Compiler and linker flags

CFLAGS	=	-O -I$(X11INC) -I$(MOTIFINC)
LFLAGS	=	-O -L$(X11LIB) -L$(MOTIFLIB)