/* hello_dialog.c -- your typical Hello World program using 
** an InformationDialog. 
*/

#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, rc, hpb, gpb; 
	void         popup_callback(Widget, XtPointer, XtPointer);
	void         exit_callback(Widget, XtPointer, XtPointer);
	
	/* callbacks for the pushbuttons -- pops up dialog */
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);   
					 
	rc = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);
	
	hpb = XmCreatePushButton (rc, "Hello", NULL, 0);
	XtAddCallback (hpb, XmNactivateCallback, popup_callback, (XtPointer) "Hello World");
	
	gpb = XmCreatePushButton (rc, "Goodbye", NULL, 0);
	XtAddCallback (gpb, XmNactivateCallback, exit_callback, NULL);
	
	XtManageChild (hpb);
	XtManageChild (gpb);
	XtManageChild (rc);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* callback for the "Hello" PushButton. 
** Popup an InformationDialog displaying the text passed as the client data parameter. 
*/
void popup_callback (Widget button, XtPointer client_data, XtPointer call_data)
{
	Widget   dialog;
	XmString xm_string;
	void     activate_callback(Widget, XtPointer, XtPointer);
	Arg      args[5];
	int      n = 0;
	char    *text = (char *) client_data;
	
	/* set the label for the dialog */
	xm_string = XmStringCreateLocalized (text);
	XtSetArg (args[n], XmNmessageString, xm_string); n++;
	
	/* Create the InformationDialog as child of button */
	dialog = XmCreateInformationDialog (button, "info", args, n); 
	
	/* no longer need the compound string, free it */
	XmStringFree (xm_string);
	
	/* add the callback routine */
	XtAddCallback (dialog, XmNokCallback, activate_callback, NULL);
	
	/* manage the MessageBox: has the side effect of displaying the XmDialogShell parent*/
	XtManageChild (dialog);
}

/*
** callback routine when the user pressed the "Goodbye" button
*/
void exit_callback (Widget w, XtPointer client_data, XtPointer call_data)
{
	exit (0);
}

/* callback routine for when the user presses the OK button. 
** Yes, despite the fact that the OK button was pressed, the 
** widget passed to this callback routine is the dialog! 
*/
void activate_callback (Widget dialog, XtPointer client_data, XtPointer call_data)
{
	puts ("OK was pressed.");
}
