/*
 * gcc -o test test.c -lX11
 * */
/*
 The X Window System was specifically designed to allow the graphical output of a program running on one machine to appear on a different machine, possibly one that is physically remote and/or a different make and architecture. In other words, X11 was designed to be a platform-independent, networked graphics framework.

In X11 parlance, the "display" denotes the box on which the graphical output will appear. Interestingly, an individual display is defined by the X11 documentation as having exactly one keyboard and one pointer (i.e., mouse), but potentially multiple CPUs, monitors, etc.

The "screen" corresponds to the actual physical display device; in most cases this will be a monitor. X11 allows for an arbitrary number of screens to be connected to each display. Think of a workstation with two monitors or a departmental server, connected to a larger number of (relatively dumb) X terminals.

Finally, a "window" is a rectangular area of the screen that can be used for input and output. If the rectangular area is not directly associated with a screen, but instead resides in memory, it is referred to as a "pixmap". Pixmaps and windows share the property of being "drawable" and can be used interchangeably in some function calls. It is important to remember that to X11 a window is merely a rectangular area on the screen. As such, it does not include things like titlebars, scrollbars and other GUI elements that we have come to associate with the word window. If these elements are present, they are controlled by a different program called a window manager.

Every GUI-oriented computer ships with a mouse or equivalent. When X11 came about, the development of graphical input devices was still in its infancy. Consequently the X11 documentation always speaks (somewhat bashfully) of a "pointer" (a generic term for mice), trackballs, digitizing tablets or other yet-to-be-invented graphical input devices. A final cause of confusion is the specific usage of the words client and server in X11: a "client" is any application that creates data for graphical output. The "server" is the program that manages the shared resource accessed by all clients, namely the (finite) amount of screen real estate. The unfortunate consequence of this naming convention is that the (X11) client typically executes on the server (machine), while the (X11) server runs on the client (computer).
*/

//Xlib.h is the most important header file for X11 programming. It defines several structs and macros used throughout an X11 program and provides function prototypes for all the basic functions in the library. Other headers are part of X11 as well. If those are needed, Xlib.h usually has to be included before any of the other headers because they depend on it. Strangely, the dependent headers do not themselves include Xlib.h.
#include <X11/Xlib.h> // preceede most other headers!

int main(){

//Display is a struct defined in Xlib.h that represents the display, i.e., the box on which the graphical output is meant to appear. The library function XOpenDisplay attempts to establish a connection to the X11 server on this machine. As argument, it takes a zero-terminated string with the display_name, in the following format: hostname:servernumber.screennumber. If the argument is NULL, the display_name defaults to the value of the environment variable DISPLAY. If no connection can be established, XOpenDisplay will return NULL.
  Display *dsp = XOpenDisplay( NULL );
  if( !dsp ){ return 1; }

//we obtain an identifier to be used for the screen. DefaultScreen is not a function; it is a macro defined in Xlib.h. X11 provides macros like this as accessors for the elements of the Display struct that are accessible to the programmer. Members of this and other library structs should never be accessed directly, only through the provided macros. Variables for which no macros are provided should be considered "private".
  int screenNumber = DefaultScreen(dsp);
  //colors in X11 are identified by integer numbers. Since X11 is meant to be platform independent, it does not make any assumptions about the capabilities of any device (i.e., the screen not the display) to render colors. Only two colors are guaranteed to be available, black and white. Note that the actual appearance of these values does not have to correspond to black and white pixels; think of those old monitors with green (or amber) letters on a black background.
  unsigned long white = WhitePixel(dsp,screenNumber);
  unsigned long black = BlackPixel(dsp,screenNumber);

//finally, we are ready to create a window. The data type Window is not a struct as one might assume. Rather it is typedefed to some integer data type and merely provides an identifier for a window.
  Window win = XCreateSimpleWindow(dsp,
                DefaultRootWindow(dsp), //each window is the child of some other window and is geometrically contained by it. Here we set the root window (the entire screen) of the default screen as the parent window.
                50, 50,   // origin: the coordinates of the upper-left-hand corner of the new window with respect to the parent window, designated in pixels. X11 uses graphical coordinates with the origin in the upper left and with the positive X-axis running to the right and the positive Y-axis running down. However, since the placement of new windows is under control of the window manager, the new window can pop up pretty much anywhere on the screen, regardless of the values of these two arguments--expect surprises.
                200, 200, // size: the width and height (in that order) of the new window, in pixels.
                0, black, // border: the width and color of the border of the window. This is not the border that may be added by the window manager.
                               white );  // backgd: the color of the background of the window.

  /*so far we have created a data structure for the window, but only in memory. In X11 a window is not automatically displayed when it is created. Making the window visible is a separate process, called mapping. The function XMapWindow consequentially maps the window on the specified display. Note that it is not necessary to specify the screen again; when the window was created, it was explicitly created as child of a parent window, which itself is associated with a specific screen.*/
  XMapWindow( dsp, win );

/*To understand the next few lines, we have to reconsider one of the basic tenets of X11: it is a networked graphics framework in which the client (such as the program in Listing 1) and the graphics server might reside on physically remote machines. In such a situation, the failure of the network is a possibility that has to be anticipated. The XMapWindow command is issued on the client but executed on the server (note how X11's way of using those terms makes much more sense in the present context), which might not be available when the command is issued. The solution to this situation is to make the client/server communication asynchronous or event-driven. That is, XMapWindow sends its request to the server and returns immediately, without waiting for the completion of the server process. It is then up to the client to poll the server for successful execution of the previous command. The following lines do exactly that.*/

/*first, we need to select what kinds of events (such as mouse clicks, mouse movements, key strokes, etc.) we are interested in. Xlib.h defines a number of bit masks for the different kinds of events that can be bitwise ORed, if we are interested in several kinds of events. Here we are interested in the success of the previous mapping operation, and we therefore select the appropriate bit mask and inform the server of our choice, using XSelectInput. This function overwrites any previous setting with each new call.*/
  long eventMask = StructureNotifyMask;
  XSelectInput( dsp, win, eventMask );

  /*XNextEvent blocks until an event occurs, sending the program to sleep so it does not consume CPU time while waiting. As a side effect, all non-empty output buffers are flushed with each call to XNextEvent, obviating explicit calls to XFlush. Since the StructureNotifyMask combines several events that are structural changes to the window and not just mapping events, we have to loop until the proper type of event has been reported. (If this presentation leaves questions, don't despair: hopefully a fuller treatment of X11 events will be the subject future articles.)*/
  XEvent evt;
  do{
    XNextEvent( dsp, &evt );   // calls XFlush
  }while( evt.type != MapNotify );

/*now that we can be certain the window is actually visible on the screen, we are finally ready to draw something to it. First, we define a GraphicsContext (GC), a struct that comprises information about the appearance (such as color, linestyle, etc.) of the graphical elements. Attributes of the GraphicsContext can be set at creation by providing a bit mask specifying the attributes as the third and an array of values as the fourth argument. Alternatively, attributes of the GC can be set individually by calling the appropriate functions.*/
  GC gc = XCreateGC( dsp, win,
                     0,        // mask of values
                     NULL );   // array of values
  XSetForeground( dsp, gc, black );

/*draw two lines, forming an X on the screen. There are similar functions to draw points, arcs and rectangles, all taking coordinates relative to the current window. Remember to use these functions only with windows that you know are actually visible, not hidden or minimized. Drawing to an invisible window has no effect.*/
  XDrawLine(dsp, win, gc, 10, 10,190,190); //from-to
  XDrawLine(dsp, win, gc, 10,190,190, 10); //from-to

/*now that the graph (as it is) is complete, the program should wait for the user to click on the window to terminate it. Accordingly, we set the appropriate event mask and wait for the desired event. Note again that XNextEvent flushes all output buffers, so we are certain that the two lines indeed appeared before the mouse-click event occurred.*/
  eventMask = ButtonPressMask|ButtonReleaseMask;
  XSelectInput(dsp,win,eventMask); // override prev
 
  do{
    XNextEvent( dsp, &evt );   // calls XFlush()
  }while( evt.type != ButtonRelease );


  /*part of well-behaved C programming is responsible resource management. We therefore explicitly free the allocated resources, rather than let the program fall off the end and have the operating system clean up after us. As a side effect, XDestroyWindow unmaps the window in question automatically, so it disappears from the screen.
   */
  XDestroyWindow( dsp, win );
  XCloseDisplay( dsp );

  return 0;
}
