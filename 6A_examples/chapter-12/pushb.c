/* pushb.c -- demonstrate the pushbutton widget. Display one
** PushButton with a single callback routine.  Print the name
** of the widget and the number of "multiple clicks".  This
** value is maintained by the toolkit.
*/

#include <Xm/PushB.h>

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, button;
	void         my_callback(Widget, XtPointer, XtPointer);
	XmString     btn_text;
	Arg          args[2];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);
					 
	btn_text = XmStringCreateLocalized ("Push Here");
	XtSetArg (args[0], XmNlabelString, btn_text);
	button = XmCreatePushButton (toplevel, "button", args, 1);
	XmStringFree (btn_text);
	
	XtAddCallback (button, XmNarmCallback, my_callback, NULL);
	XtAddCallback (button, XmNactivateCallback, my_callback, NULL);
	XtAddCallback (button, XmNdisarmCallback, my_callback, NULL);
	XtManageChild (button);
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

void my_callback (Widget w, XtPointer client_data, XtPointer call_data)
{
	XmPushButtonCallbackStruct *cbs = (XmPushButtonCallbackStruct *) call_data;

	if (cbs->reason == XmCR_ARM)
		printf ("%s: armed\n", XtName (w));
	else if (cbs->reason == XmCR_DISARM)
		printf ("%s: disarmed\n", XtName (w));
	else
		printf ("%s: pushed %d times\n", XtName (w), cbs->click_count);
}
