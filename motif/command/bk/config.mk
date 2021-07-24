#==============================================================
# Config.mk -- Configurations for all subdirectory Makefiles 
# Time-stamp: <1999-09-18 15:13:40 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

# Edit the following for your installation

CC	=	g++
X11INC	=	/usr/X11R6/include
X11LIB	=	/usr/X11R6/lib
MOTIFINC=	/usr/X11R6/LessTif/Motif1.2/include
MOTIFLIB=	/usr/X11R6/LessTif/Motif1.2/lib
LIBS	=	 -lXm -lXt -lX11 -lXext   

#==============================================================
# Compiler and linker flags

CFLAGS	=	-O -I$(X11INC) -I$(MOTIFINC)
LFLAGS	=	-O -L$(X11LIB) -L$(MOTIFLIB)
