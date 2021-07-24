PROGRAMS = rendered_list 

CFLAGS= -g
#INCLUDES = -I/fs81/objtrees/sparc-sun-sunos5.6/xd6.0af/tree/Motif2/include -I/fs81/OSF/X11/include
#LIBS= -L/fs81/objtrees/sparc-sun-sunos5.6/xd6.0af/tree/Motif2/lib -L/fs81/OSF/X11/lib -lXm -lXt -lSM -lICE -lX11 -lXext -lXp
INCLUDES = -I/usr/dt/include -I/usr/openwin/include
LIBS= -L/usr/dt/lib -L/usr/openwin/lib -lXm -lXt -lSM -lICE -lX11 -lXext

all: $(PROGRAMS)

.c: ; $(CC) $(CFLAGS) $(INCLUDES) $@.c $(LIBS) -o $@

clean::
	-rm -f $(PROGRAMS) *.o

