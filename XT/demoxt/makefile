#==============================================================
# Makefile -- Make file for Xwelcome program
# Time-stamp: <1999-08-04 13:14:35 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

CONFIG = ./config.mk
include $(CONFIG)

#==============================================================
# This program's object code files

OBJS	=	demo_xt4.o

#==============================================================
# File dependencies and rules

all:	demo_xt4

objs:	$(OBJS)

clean:
	rm -f $(OBJS)
	rm -f *~
	rm -f demo_xt4

demo_xt4: $(OBJS)
	$(CC) -o $@ $(LFLAGS) $(OBJS) $(LIBS)

demo_xt4.o:	demo_xt4.cpp
	$(CC) -c $(CFLAGS) -o $@ $<





