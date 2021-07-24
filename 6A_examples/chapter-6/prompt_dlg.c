/* prompt_dlg.c -- prompt the user for a string. Two PushButtons
** are displayed. When one is selected, a PromptDialog is displayed
** allowing the user to type a string. When done, the PushButton's
** label changes to the string.
*/

#include <Xm/SelectioB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, rc, button;
	void         pushed(Widget, XtPointer, XtPointer); 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	/* Initialize toolkit and create toplevel shell */
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);
					 
	/* RowColumn managed both PushButtons */
	rc = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);
	
	/* Create two pushbuttons -- both have the same callback */
	button = XmCreatePushButton (rc, "PushMe 1", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, pushed, NULL);
	XtManageChild (button);
	
	button = XmCreatePushButton (rc, "PushMe 2", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, pushed, NULL);
	XtManageChild (button);
	
	XtManageChild (rc);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/*
** Destroy the prompt dialog's shell parent, and thus also the prompt
*/
void destroy_dialog (Widget w, XtPointer client_data, XtPointer call_data)
{
	XtDestroyWidget (XtParent (w));
}

/* pushed() --the callback routine for the main app's pushbuttons.
** Create a dialog that prompts for a new button name.
**/
void pushed (Widget widget, XtPointer client_data, XtPointer call_data)
{
	Widget   dialog;
	XmString t = XmStringCreateLocalized ("Enter New Button Name:");
	void     read_name(Widget, XtPointer, XtPointer);
	Arg      args[5];
	int      n = 0;
	
	/* Create the dialog -- the PushButton acts as the DialogShell's
	** parent (not the parent of the PromptDialog). 
	*/
	
	XtSetArg (args[n], XmNselectionLabelString, t); n++;
	XtSetArg (args[n], XmNautoUnmanage, False); n++;
	dialog = XmCreatePromptDialog (widget, "prompt", args, n);
	XmStringFree (t);
	
	/* always destroy compound strings when done */
	/* When the user types the name, call read_name()... */
	XtAddCallback (dialog, XmNokCallback, read_name, (XtPointer) widget);
	
	/* If the user selects cancel, just destroy the dialog */
	XtAddCallback (dialog, XmNcancelCallback, destroy_dialog, NULL);
	
	/* No help is available... */
	XtSetSensitive (XtNameToWidget (dialog, "Help"), False);
	XtManageChild (dialog);
}

/* read_name() --the text field has been filled in. */
void read_name (Widget widget, XtPointer client_data, XtPointer call_data)
{
	Widget push_button = (Widget) client_data;
	XmSelectionBoxCallbackStruct *cbs = (XmSelectionBoxCallbackStruct *) call_data;
	
	XtVaSetValues (push_button, XmNlabelString, cbs->value, NULL);

	/* Name's fine -- go ahead and enter it */
	XtDestroyWidget (XtParent (widget));
}
