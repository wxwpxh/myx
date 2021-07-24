/* map_dlg.c -- Use the XmNmapCallback to automatically position
** a dialog on the screen. Each time the dialog is displayed, it
** is mapped down and to the right by 200 pixels in each direction.
*/

#include <Xm/MessageB.h>
#include <Xm/PushB.h>

/* main() --create a pushbutton whose callback pops up a dialog box */
main (int argc, char *argv[])
{
	Widget       toplevel, button; 
	XtAppContext app;
	void         pushed(Widget, XtPointer, XtPointer); 

	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	button = XmCreatePushButton (toplevel, "Push Me", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, pushed, (XtPointer) "Hello World");
	XtManageChild (button);

	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* callback function for XmNmapCallback.  Position dialog in 200 pixel
** "steps".  When the edge of the screen is hit, start over.
*/
static void map_dialog (Widget dialog, XtPointer client_data, XtPointer call_data)
{
	static Position x, y;
	Dimension       w, h;
	
	XtVaGetValues(dialog, XmNwidth, &w, XmNheight, &h, NULL);
	
	if ((x + w) >= WidthOfScreen (XtScreen (dialog)))
		x = 0;
		
	if ((y + h) >= HeightOfScreen (XtScreen (dialog)))
		y = 0;
		
	XtVaSetValues (dialog, XmNx, x, XmNy, y, NULL);
	
	x += 200;
	y += 200;
}

/* pushed() --the callback routine for the main app's pushbutton. 
** Create and popup a dialog box that has callback functions for 
** the Ok, Cancel and Help buttons. 
*/
void pushed (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget   dialog;
	Arg      arg[5];
	int      n = 0;
	char    *message = (char *) client_data; 
	XmString t = XmStringCreateLocalized (message);

	XtSetArg (arg[n], XmNmessageString, t); n++;
	XtSetArg (arg[n], XmNdefaultPosition, False); n++; 
	dialog = XmCreateMessageDialog (w, "notice", arg, n);
	XmStringFree (t);

	XtAddCallback (dialog, XmNmapCallback, map_dialog, NULL);
	XtManageChild (dialog);
}
