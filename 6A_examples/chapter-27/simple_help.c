/* simple_help.c -- create a PushButton that posts a dialog box
** that entices the user to press the help button. The callback
** for this button displays a new dialog that gives help.
*/

#include <Xm/MessageB.h>
#include <Xm/PushB.h>

main (int argc, char *argv[])
{
	Widget       toplevel, button;
	XtAppContext app;
	XmString     label;
	Arg          args[2];
	void         pushed(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	
	label = XmStringCreateLocalized ("Push Me");
	XtSetArg (args[0], XmNlabelString, label);
	button = XmCreatePushButton (toplevel, "button",args, 1);
	XtAddCallback (button, XmNactivateCallback, pushed,
	               "You probably need help for this item.");
	XmStringFree (label);

	XtManageChild (button);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

#define HELP_TEXT "This is the help information.\nNow press 'OK'"

/* pushed() -- the callback routine for the main app's pushbutton. */
void pushed (Widget w, XtPointer client_data, XtPointer call_data)
{
	char    *text = (char *) client_data;
	Widget   dialog;
	XmString t = XmStringCreateLocalized (text);
	Arg      args[5];
	int      n;
	void     help_callback(Widget, XtPointer, XtPointer);
	void     help_done(Widget, XtPointer, XtPointer);
	
	n = 0;
	XtSetArg (args[n], XmNautoUnmanage, False); n++;
	XtSetArg (args[n], XmNmessageString, t); n++;
	dialog = XmCreateMessageDialog (XtParent(w), "notice", args, n);
	XmStringFree (t);
	XtUnmanageChild ( XtNameToWidget (dialog, "Cancel"));
	XtAddCallback (dialog, XmNokCallback, help_done, NULL);
	XtAddCallback (dialog, XmNhelpCallback, help_callback, HELP_TEXT);
	
	/* This also pops up the DialogShell parent */
	XtManageChild (dialog);
}

/* help_callback() -- callback routine for the Help button in the
** original dialog box that displays an InformationDialog based on the 
** help_text parameter.
*/

void help_callback (Widget parent, XtPointer client_data, XtPointer call_data)
{
	char    *help_text = (char *) client_data;
	Widget   dialog;
	XmString text; 
	void     help_done(Widget, XtPointer, XtPointer);
	Arg      args[5];
	int      n;
	
	n = 0;
	text = XmStringGenerate ((XtPointer) help_text,
				 XmFONTLIST_DEFAULT_TAG,
				 XmCHARSET_TEXT,
				 NULL);

	XtSetArg (args[n], XmNmessageString, text); n++;
	XtSetArg (args[n], XmNautoUnmanage, False); n++;
	dialog = XmCreateInformationDialog (parent, "help", args, n);
	XmStringFree (text);
	
	XtUnmanageChild ( XtNameToWidget (dialog, "Cancel"));
	XtSetSensitive ( XtNameToWidget (dialog, "Help"), False);

	/* the OK button will call help_done() below */
	XtAddCallback (dialog, XmNokCallback, help_done, NULL);
	/* display the help text */
	XtManageChild (dialog);
}

/* help_done() -- called when user presses "OK" in dialogs. */
void help_done(Widget dialog, XtPointer client_data, XtPointer call_data)
{
	XtDestroyWidget (XtParent (dialog));
}
