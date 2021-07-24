/* Written by Ch. Tronche (http://tronche.lri.fr:8000/)
  Copyright by the author. This is unmaintained, no-warranty free software. 
  Please use freely. It is appreciated (but by no means mandatory) to
  acknowledge the author's contribution. Thank you.
  Started on Thu Jun 26 23:29:03 1997

 
  Xlib tutorial: 1st program
  Make a window appear on the screen.
  
gcc -o  prog-4 prog-4.c -lX11

*/


#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <stdio.h>
#include <stdlib.h>

 

Window *list(Display *disp, unsigned long *len)
{
    Atom prop = XInternAtom(disp, "_NET_CLIENT_LIST", False), type;
    int form;
    unsigned long remain;
    unsigned char *list;

    XGetWindowProperty(disp, XDefaultRootWindow(disp), prop, 0, 1024, False, XA_WINDOW,
                            &type, &form, &len, &remain, &list);
    return (Window *)list;
}

char *name(Display *disp, Window window)
{
    Atom prop = XInternAtom(disp, "WM_NAME", False), type;
    int form;
    unsigned long remain, len;
    unsigned char *list;

    XGetWindowProperty(disp, window, prop, 0, 1024, False, AnyPropertyType,
                            &type, &form, &len, &remain, &list);
    return (char*)list;
}

int main(int argc, char *argv[])
{
    Display *disp;
    Window *wlist;
    unsigned long len;
    char *wname;

    disp = XOpenDisplay(NULL);
    wlist = (Window*)list(disp, &len);

    int i;
    for(i = 0; i < (int)len; i++){
            if(wlist[i] != 0){
                    wname = name(disp, wlist[i]);
                    printf("%d: %s\n", i, wname);
                    free(wname);
            }
    }
return 0;

}
