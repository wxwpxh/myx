/* Written by Ch. Tronche (http://tronche.lri.fr:8000/)
  Copyright by the author. This is unmaintained, no-warranty free software. 
  Please use freely. It is appreciated (but by no means mandatory) to
  acknowledge the author's contribution. Thank you.
  Started on Thu Jun 26 23:29:03 1997

 
  Xlib tutorial: 1st program
  Make a window appear on the screen.
  
gcc -o  demo4 demo4.c -lX11

*/

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
/*
#include <locale.h>
*/
#include <X11/Xlocale.h>
#include <stdlib.h>

void DrawCenteredMbString(Display *, Window, XFontSet, GC, char *, int, int, int, int, int);

int main(int argc, char *argv[]) {
    Display *dpy;
    Window w;
    XFontSet fontset;
    char **missing_charsets;
    int num_missing_charsets;
    char *default_string;
    int i;
    GC gc;
    XEvent xe;

    if (setlocale(LC_ALL, "") == NULL) {
        printf("cannot set locale \n");
        exit(1);
    }

    if (!XSupportsLocale()) {
        printf("X does not support locale %s\n",setlocale(LC_ALL, NULL));
        exit(1);
    }

    if (XSetLocaleModifiers("") == NULL) {
        printf("Warning: cannot set locale modifiers\n");
    }

    dpy = XOpenDisplay(NULL);
    w = XCreateSimpleWindow(
            dpy, RootWindow(dpy, 0), 50, 50,
                            300, 200, 5, BlackPixel(dpy, 0),
                            WhitePixel(dpy, 0));
    gc = XCreateGC(dpy, w, 0L, (XGCValues *) NULL);

    fontset = XCreateFontSet(
            dpy, "-*-*-*-*-*-*-16-*-*-*-*-*-*-*",
                             &missing_charsets, &num_missing_charsets,
                             &default_string);
    if (num_missing_charsets > 0) {
        printf("The following charsets are missing\n");
        for (i = 0; i < num_missing_charsets; i++) {
            printf("%s \n", missing_charsets);
            printf("The string is %s \n", default_string);
            printf("of any characters from those sets\n");
        }
        XFreeStringList(missing_charsets);
    }
    XSetLineAttributes(dpy, gc, 5, LineSolid, CapRound, JoinRound);
    XSelectInput(
            dpy, w, ExposureMask | ButtonPressMask
                         | EnterWindowMask | LeaveWindowMask);
    XMapWindow(dpy, w);
    XFlush(dpy);
    while (1) {
        XNextEvent(dpy, &xe);
        switch (xe.type) {
            case Expose:
                printf("Expose 出现\n");
                XSetForeground(dpy, gc, BlackPixel(dpy, 0));
                DrawCenteredMbString(
                        dpy, w, fontset, gc,
                                     "你好 Hello",
                                     10, 0, 0, 100, 50);
                break;
            case ButtonPress:
                printf("按下\n");
//                exit(1);
                break;
            case EnterNotify:
                printf("进入\n");
                XSetForeground(dpy, gc, BlackPixel(dpy, 0));
                XDrawRectangle(dpy, w, gc, 0, 0, 100, 50);
                break;
            case LeaveNotify:
                printf("离开\n");
                XSetForeground(dpy, gc, WhitePixel(dpy, 0));
                XDrawRectangle(dpy, w, gc, 0, 0, 100, 50);
                break;
            default:
                printf("默认\n");
                break;
        }
    }
    return 0;
}

void
DrawCenteredMbString(Display *dpy, Window w, XFontSet fontset, GC gc, char *str, int num_bytes, int x, int y, int width,
                     int height) {
    XRectangle boundingbox;
    XRectangle dummy;
    int originx, originy;

    (void) XmbTextExtents(fontset, str, num_bytes, &dummy, &boundingbox);

    originx = x + (width - boundingbox.width) / 2 - boundingbox.x;
    originy = y + (height - boundingbox.height) / 2 - boundingbox.y;

    XmbDrawString(dpy, w, fontset, gc, originx, originy, str, num_bytes);
}

