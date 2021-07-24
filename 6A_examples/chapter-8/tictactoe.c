/* tictactoe.c -- demonstrate how fractionBase and XmATTACH_POSITIONs
** work in Form widgets.
*/

#include <Xm/PushB.h>
#include <Xm/Form.h>

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, parent, w;
	int          x, y, n;
	Arg          args[10];
	void         pushed(Widget, XtPointer, XtPointer); /* callback for each PushButton */
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	n = 0;
	XtSetArg (args[n], XmNfractionBase, 3); n++;
	parent = XmCreateForm (toplevel, "form",   args, n);
	
	for (x = 0; x < 3; x++)
		for (y = 0; y < 3; y++) {
			n = 0;
			XtSetArg (args[n], XmNtopAttachment, XmATTACH_POSITION); n++;
			XtSetArg (args[n], XmNtopPosition, y); n++;
			XtSetArg (args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
			XtSetArg (args[n], XmNleftPosition, x); n++;
			XtSetArg (args[n], XmNrightAttachment, XmATTACH_POSITION); n++;
			XtSetArg (args[n], XmNrightPosition, x+1); n++;
			XtSetArg (args[n], XmNbottomAttachment, XmATTACH_POSITION); n++; 
			XtSetArg (args[n], XmNbottomPosition, y+1); n++;
			w = XmCreatePushButton (parent, "   ",  args, n);
			XtAddCallback (w, XmNactivateCallback, pushed, NULL);
			XtManageChild (w);
		}
		
	XtManageChild (parent);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

void pushed ( Widget    w, /* The PushButton that got activated */
	      XtPointer client_data, /* unused -- NULL was passed to XtAddCallback() */
	      XtPointer call_data)
{
	char     buf[2];
	XmString str;
	XmPushButtonCallbackStruct *cbs = (XmPushButtonCallbackStruct *) call_data;
	
	/* Shift key gets an O. (xbutton and xkey happen to be similar) */
	
	if (cbs->event->xbutton.state & ShiftMask)
		buf[0] = '0';
	else
		buf[0] = 'X';
	buf[1] = 0;
	
	str = XmStringCreateLocalized (buf);
	
	XtVaSetValues (w, XmNlabelString, str, NULL);
	XmStringFree (str);
}
