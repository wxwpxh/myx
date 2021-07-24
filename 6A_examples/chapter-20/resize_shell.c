/* resize_shell.c -- demonstrate the max and min heights and widths.
** This program should be run to really see how mwm displays the
** size of the window as it is resized.
*/

#include <Xm/PushB.h>

main (int argc, char *argv[])
{
	Widget       toplevel, button;
	XtAppContext app;
	void         getsize(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass,
					XmNminWidth, 75,
					XmNminHeight, 25,
					XmNmaxWidth, 150,
					XmNmaxHeight, 100,
					XmNbaseWidth, 5,
					XmNbaseHeight, 5,
					XmNwidthInc, 5,
					XmNheightInc, 5,
					NULL);
					
	/* Pushbutton's callback prints the dimensions of the shell. */
	button = XmCreatePushButton (toplevel, "Print Size", NULL, 0);
	XtManageChild (button);
	XtAddCallback (button, XmNactivateCallback, getsize, (XtPointer) toplevel);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

void getsize (Widget widget, XtPointer client_data, XtPointer call_data)
{
	Widget shell = (Widget) client_data;
	Dimension width, height;
	
	XtVaGetValues (shell, XmNwidth, &width, XmNheight, &height, NULL);
	
	printf ("Width = %d, Height = %d\n", width, height);
}
