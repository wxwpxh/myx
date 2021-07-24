/* Written by Ch. Tronche (http://tronche.lri.fr:8000/)
  Copyright by the author. This is unmaintained, no-warranty free software. 
  Please use freely. It is appreciated (but by no means mandatory) to
  acknowledge the author's contribution. Thank you.
  Started on Thu Jun 26 23:29:03 1997

 
  Xlib tutorial: 1st program
  Make a window appear on the screen.
  
gcc -o  x-hello x-hello.c -lX11

*/

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
	Display *d;
	Window w;
	XEvent e;
	char *msg = "Hello, World!";
	int s;

    d = XOpenDisplay(NULL);
	if (d == NULL) {
		fprintf(stderr, "Cannot open display\n");
		exit(1);
		} 
	s = DefaultScreen(d);
	w = XCreateSimpleWindow(d, RootWindow(d, s), 100, 100, 500,500, 1,777215, 111111);
	printf("BlackPixel(d, s) is %d\n",(int)BlackPixel(d, s));
	printf("WhitePixel(d, s) is %d\n",(int)WhitePixel(d, s));
	XSelectInput(d, w, ExposureMask | KeyPressMask);
	XMapWindow(d, w);
    while (1) {
		XNextEvent(d, &e);
		if (e.type == Expose) {
			XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
			XDrawString(d, w, DefaultGC(d, s), 10, 50, msg, strlen(msg));
			}
		if (e.type == KeyPress)
			break;
	}
	
	XCloseDisplay(d);
	return 0;
}