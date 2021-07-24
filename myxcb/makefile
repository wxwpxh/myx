#==============================================================
# Makefile -- Make file for Xwelcome program
# Time-stamp: <1999-08-04 13:14:35 tswan>
# Copyright (c) 1999 by Tom Swan. All rights reserved.
#==============================================================

CONFIG = ./config.mk
include $(CONFIG)

#==============================================================
# This program's code files
BIGprg	=	1
OBJS	=	1.o  editdialog.o filetotext.o scrolllist.o	
cprg	=	hello1  hello2 hello3 hello4 hello5 prog1 prog \
		testxcb
cxxprg	=
#==============================================================
# File dependencies and rules

all:	$(cprg) $(BIGprg)

$(BIGprg): $(OBJS)
	$(CXX) -o $@ $(LFLAGS) $(OBJS) $(LIBS)

$(cprg): % : %.o
	$(CC)  -o $@ $< $(LFLAGS) $(LIBS)

$(cxxprg): % : %.o 
	$(CXX) -o $@ $< $(LFLAGS) $(LIBS)

clean:
	rm -f *~
	rm -f *.o
	rm -f core

cleanall:clean
	rm -f $(cprg)
	rm -f $(BIGprg)
	rm -f $(cxxprg)
