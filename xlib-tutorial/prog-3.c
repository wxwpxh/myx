/* Written by Ch. Tronche (http://tronche.lri.fr:8000/)
  Copyright by the author. This is unmaintained, no-warranty free software. 
  Please use freely. It is appreciated (but by no means mandatory) to
  acknowledge the author's contribution. Thank you.
  Started on Thu Jun 26 23:29:03 1997

 
  Xlib tutorial: 1st program
  Make a window appear on the screen.
  
gcc -o  prog-3 prog-3.c -lX11

*/

#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xmu/WinUtil.h>
//#include <X11/Xutil.h>

int main(void)
{
 Display *display;
 Window window;
 XSetWindowAttributes attr;
 XSizeHints *sz;

 /* create a display connection */
 display = XOpenDisplay(NULL);
 if (!display) {
 printf("NullPointerException\n");
 return 1;
 }
 /* create and set attributes of window */
 window = XCreateWindow(display, XDefaultRootWindow(display),
 100, 100, 300, 300, 2, XDefaultDepth(display, 0),
 InputOutput, CopyFromParent, 0, &attr);

 /* communicate with Window Manager */
 XStoreName(display, window, "Hello world!");
 sz = XAllocSizeHints();
 sz->x = 100;
 sz->y = 100;
 sz->width = 300;
 sz->height = 300;
 sz->flags = (1L<<0) | (1L<<1);
 XSetNormalHints(display, window, sz);
 XSetIconName(display, window, "-----");

 /* mapping window */
 printf("Map window\n");
 XMapWindow(display, window);
 getchar();

 printf("1XFlush\n");
 XFlush(display);
 getchar();

 sleep(10);

 /* close window */
 printf("Destroy Window\n");
 XDestroyWindow(display, window);
 getchar();

 printf("2XFlush\n");
 XFlush(display);
 getchar();

 /* close display */
 printf("close display\n");
 XCloseDisplay(display);
 getchar();

 return 0;
}