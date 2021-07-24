#==============================================================
# Makefile -- Make file for Xwelcome program
# Time-stamp: <1999-08-04 13:14:35 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

CONFIG = ./config.mk
include $(CONFIG)

#==============================================================
# This program's object code files

OBJS	=	1.o  editdialog.o filetotext.o scrolllist.o

#==============================================================
# File dependencies and rules

all:	1

objs:	$(OBJS)

clean:
	rm -f $(OBJS)
	rm -f *~
	rm -f 1
	rm -f core

1: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LFLAGS)


