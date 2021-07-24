/* Written by Ch. Tronche (http://tronche.lri.fr:8000/)
  Copyright by the author. This is unmaintained, no-warranty free software. 
  Please use freely. It is appreciated (but by no means mandatory) to
  acknowledge the author's contribution. Thank you.
  Started on Thu Jun 26 23:29:03 1997

 
  Xlib tutorial: 1st program
  Make a window appear on the screen.
  
gcc -o  demo3 demo3.c -lX11

*/

#include <X11/Xlib.h>
#include <stdio.h>

int main() {
    Display *d;
    Window w, wb;
    XEvent event;
    Font f;
    GC gc;
    d = XOpenDisplay(NULL);
    w = XCreateSimpleWindow(
            d, RootWindow(d, 0),
            180, 50,
            260, 160,
            1, WhitePixel(d, 0), BlackPixel(d, 0));
    XSelectInput(d, w, ExposureMask | ButtonPressMask);

    wb = XCreateSimpleWindow(
            d, w,
            20, 20,
            100, 20,
            1, WhitePixel(d, 0), BlackPixel(d, 0));

    XSelectInput(d, wb, ExposureMask | ButtonPressMask);
    gc = XCreateGC(d, w, 0, NULL);
    f = XLoadFont(d, "fixed");
    XSetFont(d, gc, f);
    XMapWindow(d, w);
    XMapWindow(d, wb);
    XFlush(d);
    while (1) {
        // 拿到Event
        XNextEvent(d, &event);
        switch (event.type) {
            case Expose:
                if (event.xany.window == wb) {
                    XSetForeground(d, gc, BlackPixel(d, 0));
                    XFillRectangle(d, w, gc, 0, 0, 300, 200);
                    XSetForeground(d, gc, WhitePixel(d, 0));
                    XDrawString(d, wb, gc, 10, 10, "Button", 6);//把push换成中文例如”确 //定”？？？？？为什么编译完后，执行，不能出现中文

                }
                break;
            case ButtonPress:
                if (event.xany.window == w) {
                    printf("w ");
                    fflush(stdout);
                }
                if (event.xany.window == wb) {
                    printf("wb ");
                    fflush(stdout);
                }
                break;
        }
    }
}



