/* reason.c -- examine the reason field of the callback structure 
** passed as the call_data of the callback function. This field 
** indicates which action area button in the dialog was pressed. 
*/

#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

/* main() --create a pushbutton whose callback pops up a dialog box */
main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, rc, pb1, pb2;
	void         pushed(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);
					 
	rc = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);
	pb1 = XmCreatePushButton (rc, "Hello", NULL, 0);
	XtAddCallback (pb1, XmNactivateCallback, pushed, (XtPointer) "Hello World");

	pb2 = XmCreatePushButton (rc, "Goodbye", NULL, 0);
	XtAddCallback (pb2, XmNactivateCallback, pushed, (XtPointer) "Goodbye World");

	XtManageChild (pb1);
	XtManageChild (pb2);
	XtManageChild (rc);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* pushed() --the callback routine for the main app's pushbuttons. 
** Create and popup a dialog box that has callback functions for 
** the OK, Cancel and Help buttons. 
*/
void pushed (Widget widget, XtPointer client_data, XtPointer call_data)
{
	static Widget dialog = (Widget) 0;
	char         *message = (char *) client_data;
	XmString      t = XmStringCreateLocalized (message);
	
	/* See if we've already created this dialog -- if so, 
	** we don't need to create it again. Just set the message 
	** and manage it (pop it up).     
	*/
	
	if (!dialog) {
		void callback(Widget, XtPointer, XtPointer); 
		Arg args[5];
		int n = 0;
		
		XtSetArg (args[n], XmNautoUnmanage, False); n++;
		dialog = XmCreateMessageDialog (widget, "notice", args, n);

		XtAddCallback (dialog, XmNokCallback, callback, (XtPointer) "Hi");
		XtAddCallback (dialog, XmNcancelCallback, callback, (XtPointer) "Foo");
		XtAddCallback (dialog, XmNhelpCallback, callback, (XtPointer) "Bar");
	}
	
	XtVaSetValues (dialog, XmNmessageString, t, NULL);
	XmStringFree (t);
	
	/* Managing child of DialogShell pops up the dialog */
	XtManageChild (dialog);
}

/* callback() --One of the dialog buttons was selected.
** Determine which one by examining the "reason" parameter. 
*/
void callback (Widget widget, XtPointer client_data, XtPointer call_data)
{
	char *button  = (char *) 0 ;
	char *message = (char *) client_data;
	XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;
	
	switch (cbs->reason) {
		case XmCR_OK     : button = "OK"; break;
		case XmCR_CANCEL : button = "Cancel"; break;
		case XmCR_HELP   : button = "Help"; break;
	}
	
	printf ("%s was selected: %s\n", button, message);
	
	if (cbs->reason != XmCR_HELP) { 
		/* the ok and cancel buttons "close" the widget:.
		** Unmanaging child of DialogShell pops down the dialog.
		*/
		
		XtUnmanageChild (widget);
	}
}
