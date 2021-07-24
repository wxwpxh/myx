/* multi_click.c -- demonstrate handling multiple PushButton clicks.
** First, obtain the time interval of what constitutes a multiple
** button click from the display and pass this as the client_data
** for the button_click() callback function.  In the callback, single
** button clicks set a timer to expire on that interval and call the
** function process_clicks().  Double clicks remove the timer and
** just call process_clicks() directly.
*/

#include <Xm/PushB.h>

XtAppContext app;

main (int argc, char *argv[])
{
	Widget   toplevel, button;
	void     button_click(Widget, XtPointer, XtPointer);
	XmString btn_text;
	int      interval;
	Arg      args[2];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	/* get how long for a double click */
	interval = XtGetMultiClickTime (XtDisplay (toplevel));
	printf ("Interval = %d\n", interval);

	btn_text = XmStringCreateLocalized ("Push Here");
	XtSetArg (args[0], XmNlabelString, btn_text);
	button = XmCreatePushButton (toplevel, "button", args, 1);
	XtManageChild (button);
	XmStringFree (btn_text);
	XtAddCallback (button, XmNactivateCallback, button_click, (XtPointer) interval);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* Process button clicks.  Single clicks set a timer, double clicks
** remove the timer, and extended clicks are ignored.
*/
void button_click (Widget w, XtPointer client_data, XtPointer call_data)
{
	static XtIntervalId         id       = (XtIntervalId) 0 ;
	int                         interval = (int) client_data;
	XmPushButtonCallbackStruct *cbs      = (XmPushButtonCallbackStruct *) call_data;
	void                        process_clicks(XtPointer, XtIntervalId *);

	if (cbs->click_count == 1)
		id = XtAppAddTimeOut (app, (unsigned long) interval, process_clicks, (XtPointer) False);
	else if (cbs->click_count == 2) {
		XtRemoveTimeOut (id);
		process_clicks ((XtPointer) True, (XtIntervalId *) 0);
	}
}

/* This function won't be called until we've established whether
** or not a single or a double click has occured.
*/
void process_clicks (XtPointer client_data, XtIntervalId *id)
{
	int double_click = (int) client_data;

	if (double_click)
		puts ("Double click");
	else
		puts ("Single click");
}
