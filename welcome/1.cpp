//==============================================================
// xwelcome.cpp -- Demonstrates basic X programming
// Time-stamp: <1999-08-09 10:01:46 tswan>
// To compile:
//   g++ -o 1 -L/usr/X11/lib -lX11 xwelcome.cpp
// Or:
//   Make
// To run:
//   ./xwelcome  (Press any key to exit)
// Copyright (c) 1999 by Tom Swan. All rights reserved.
//==============================================================

#include <iostream.h>
#include <stdlib.h>     // Need exit()

#include <X11/Xlib.h>   // Most X programs need these headers
#include <X11/Xutil.h>
#include <X11/Xos.h>

Display *display;       // X server display structure

int main()
{
  // Attempt to extablish communications with the X Server
  //
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    cerr << "Failure connecting to X Server"; 
    exit(1);
  }

  // Calculate window's dimensions and position
  //
  int intScreenNum = DefaultScreen(display);
  int intDisplayWidth = DisplayWidth(display, intScreenNum);
  int intDisplayHeight = DisplayHeight(display, intScreenNum);
  int intWidth = intDisplayWidth / 3;
  int intHeight = intDisplayHeight /8 ;
  int intXPos =  0;
  int intYPos =  0;

  // Get the most basic of colors (black and white)
  //
  int intBlackColor, intWhiteColor;
  intBlackColor = BlackPixel(display, intScreenNum);
  intWhiteColor = WhitePixel(display, intScreenNum);

  // Create window (this does not make it visible)
  Window win = XCreateSimpleWindow(
    display,                         // X server display struct
    DefaultRootWindow(display),      // Parent window
    intXPos, intYPos,                // Position
    intWidth, intHeight,             // Window width & height
    100,                               // Border width (default)
    intBlackColor, intWhiteColor);   // Background, foreground 


  // Assign a name for the window title bar (probably)
  XStoreName(display, win, "wxw first X program!");


  // Map the window to the display
  // Window is visible when first Expose event is received
  //
  XMapWindow(display, win);

  // Specify the types of events we want to receive
  //
  XSelectInput(display, win, 
    ExposureMask | KeyPressMask | StructureNotifyMask);

  // The infamous event loop
  //
  bool done = false;
  XEvent event;
  while (!done) {
    XNextEvent(display, &event);
    switch (event.type) {
      case Expose:
        cout << "Expose event received" << endl;
        break;
      case ConfigureNotify:
	cout << "ConfigureNotify event received" << endl;
	break;
      case KeyPress:
        cout << "Keypress detected" << endl;
        //done = true;   // Exit on any keypress
        break;
    }
  }

  // Disconnect X server connection and exit.
  // (Program may never get to here)
  //
  XCloseDisplay(display);
  return 0;
}


