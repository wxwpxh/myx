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

#include <stdio.h>
#include <stdlib.h>     // Need exit()
 
#include <Xm/Xm.h>
#include <Xm/Label.h> 

int main(int argc,char *argv[])
{
  Widget toplevel,label;
  XmString string;
  Arg args[1];

  toplevel=XtInitialize(argv[0],"LR",NULL,0,&argc,argv);
  string=XmStringCreateSimple("Hellow ,I love you!");
  XtSetArg(args[0],XmNlabelString,string);
  label=XmCreateLabel(toplevel,"label",args,1);
  XtManageChild(label);
  XtRealizeWidget(toplevel);
  XtMainLoop();
  return 0;  

}

