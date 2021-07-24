/* tick_marks.c -- demonstrate a scale widget with tick marks. */

#include <Xm/Scale.h>

main (int argc, char *argv[])
{    
	Widget        toplevel, scale;    
	XtAppContext  app;    
	int           n;    
	Arg           args[8];
	XmString      xms;

	XtSetLanguageProc (NULL, NULL, NULL);    
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);    

	xms = XmStringCreateLocalized ("Process Load");
	n = 0;
	XtSetArg (args[n], XmNtitleString, xms); n++;
	XtSetArg (args[n], XmNminimum, 0); n++;
	XtSetArg (args[n], XmNmaximum, 200); n++;
	XtSetArg (args[n], XmNvalue, 100); n++;
	XtSetArg (args[n], XmNshowValue, True); n++;

	scale = XmCreateScale (toplevel, "load", args, n);

	XmScaleSetTicks(scale, 100, 9, 1, 16, 8, 4);

	XtManageChild (scale) ;
	XtRealizeWidget (toplevel);    
	XtAppMainLoop (app); 
}

