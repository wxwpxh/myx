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
LIBS	=	-lXm -lXaw -lXt -lXext -lXmu -lX11  

#==============================================================
# Compiler and linker flags

CFLAGS	=	-O -I$(X11INC) 
LFLAGS	=	-O -L$(X11LIB) 
