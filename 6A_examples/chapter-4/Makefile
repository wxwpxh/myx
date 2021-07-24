PROGRAMS = cmd_area dynapix show_pix

CFLAGS=
INCLUDES = -I/usr/include 
LIBS= -L/usr/lib -lXm -lXt -lSM -lICE -lX11

all: $(PROGRAMS)

.c: ; $(CC) $(CFLAGS) $(INCLUDES) $@.c $(LIBS) -o $@

clean::
	-rm -f $(PROGRAMS) *.o

