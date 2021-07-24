/* set_minimum.c -- demonstrate how to set the minimum size of a
** window to its initial size. This method is useful if your program
** is initially displayed at its minimum size, but it would be too
** difficult to try to calculate ahead of time what the initial size
** would be.
*/

#include <Xm/PushB.h>

void getsize(Widget, XtPointer, XtPointer);
void configure(Widget , XtPointer , XEvent *, Boolean *);

main (int argc, char *argv[])
{
	Widget       toplevel, button;
	XtAppContext app;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass,
					XmNmaxWidth, 150,
					XmNmaxHeight, 100,
					XmNbaseWidth, 5,
					XmNbaseHeight, 5,
					XmNwidthInc, 5,
					XmNheightInc, 5,
					NULL);
					
	/* Add an event handler to trap the first configure event */
	XtAddEventHandler (toplevel, StructureNotifyMask, False, configure, NULL);
	
	/* Pushbutton's callback prints the dimensions of the shell. */
	button = XmCreatePushButton (toplevel, "Print Size", NULL, 0);
	XtManageChild (button);
	XtAddCallback (button, XmNactivateCallback, getsize, (XtPointer) toplevel);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

void getsize (Widget widget, XtPointer client_data, XtPointer call_data)
{
	Widget    shell = (Widget) client_data;
	Dimension width, height;
	
	XtVaGetValues (shell, XmNwidth, &width, XmNheight, &height, NULL);
	
	printf ("Width = %d, Height = %d\n", width, height);
}

void configure (Widget shell, XtPointer client_data, XEvent *event, Boolean *unused)
{
	XConfigureEvent *cevent = (XConfigureEvent *) event;
	
	if (cevent->type != ConfigureNotify)
		return;
		
	printf ("Width = %d, Height = %d\n", cevent->width, cevent->height);
	
	XtVaSetValues (shell, XmNminWidth, cevent->width, 
			      XmNminHeight, cevent->height, NULL);
			      
	XtRemoveEventHandler (shell, StructureNotifyMask, False, configure, NULL);
}
