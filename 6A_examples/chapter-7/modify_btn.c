/* modify_btn.c -- demonstrate how a default Motif dialog can be 
** modified to support additional items that extend the usability 
** of the dialog itself. This is a modification of the prompt_dlg.c 
** program. 
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

/* pushed() --the callback routine for the main app's pushbuttons.
** Create a dialog that prompts for a new button name or color.
** A RadioBox is attached to the dialog. Which button is selected 
** in this box is held as an int (0 or 1) in the XmNuserData resource
** of the dialog itself. This value is changed when selecting either
** of the buttons in the ToggleBox and is queried in the dialog's
** XmNokCallback function.
*/
void pushed (Widget pb, XtPointer client_data, XtPointer call_data)
{
	Widget   dialog, toggle_box;
	XmString t, btn1, btn2;
	void     read_name(Widget, XtPointer, XtPointer);
	void     toggle_callback(Widget, XtPointer, XtPointer);
	void     destroy_callback(Widget, XtPointer, XtPointer);
	Arg      args[5];
	int      n = 0;
	
	/* Create the dialog -- the PushButton acts as the DialogShell's
	** parent (not the parent of the PromptDialog). The "userData"
	** is used to store the value 
	*/
	
	t = XmStringCreateLocalized ("Enter New Button Name:");
	XtSetArg (args[n], XmNselectionLabelString, t); n++;
	XtSetArg (args[n], XmNautoUnmanage, False); n++;
	XtSetArg (args[n], XmNuserData, 0); n++;
	dialog = XmCreatePromptDialog (pb, "notice_popup", args, n);
	XmStringFree (t); /* always destroy compound strings when done */
	
	/* When the user types the name, call read_name()... */
	XtAddCallback (dialog, XmNokCallback, read_name, pb);

	/* If the user selects cancel, just destroy the dialog */
	XtAddCallback (dialog, XmNcancelCallback, destroy_callback, NULL);
	
	/* No help is available... */
	XtUnmanageChild (XtNameToWidget (dialog, "Help"));
	
	/* Create a toggle box -- callback routine is toggle_callback() */
	btn1 = XmStringCreateLocalized ("Change Name");
	btn2 = XmStringCreateLocalized ("Change Color");
	toggle_box = XmVaCreateSimpleRadioBox (dialog, "radio_box", 0 /* inital value */, toggle_callback,
					       XmVaRADIOBUTTON, btn1, 0, NULL, NULL,
					       XmVaRADIOBUTTON, btn2, 0, NULL, NULL,
					       NULL);
					       
	XtManageChild (toggle_box);
	XtManageChild (dialog);
}

/*
** This is passed the Prompt widget. We destroy the shell parent.
*/

void destroy_callback (Widget prompt, XtPointer client_data, XtPointer call_data)
{
	XtDestroyWidget ( XtParent (prompt));
}

/* callback for the items in the toggle box -- the "client data" i
** the item number selected. Since the function gets called whenever
** either of the buttons changes from true to false or back again,
** it will always be called in pairs -- ignore the "False" settings.
** When cbs->set is true, set the dialog's label string accordingly.
*/
void toggle_callback (Widget toggle_box, XtPointer client_data, XtPointer call_data)
{
	Widget   dialog = XtParent (XtParent (toggle_box));
	XmString str;
	int      n = (int) client_data;
	XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct *) call_data;
	
	if (cbs->set == XmUNSET)
		return; /* wait for the one that toggles "on" */
		
	if (n == 0)
		str = XmStringCreateLocalized ("Enter New Button Name:");
	else
		str = XmStringCreateLocalized ("Enter Text Color:");
		
	XtVaSetValues (dialog, XmNselectionLabelString, str,
			       XmNuserData, n, /* reset the user data to reflect new value */
			       NULL);
			       
	XmStringFree (str);
}

/* read_name() --the text field has been filled in. Get the userData
** from the dialog widget and set the PushButton's name or color.
*/
void read_name (Widget dialog, XtPointer client_data, XtPointer call_data)
{
	char  *text;
	int    n;
	Widget push_button = (Widget) client_data;
	
	XmSelectionBoxCallbackStruct *cbs = (XmSelectionBoxCallbackStruct *) call_data;
	
	/* userData: n == 0 -> Button Label, n == 1 -> Button Color */
	XtVaGetValues (dialog, XmNuserData, &n, NULL);
	
	if (n == 0)
		XtVaSetValues (push_button, XmNlabelString, cbs->value, NULL);
	else {
		/* convert compound string into regular text string */
		text = (char *) XmStringUnparse (cbs->value, XmFONTLIST_DEFAULT_TAG, 
						 XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL);
						 
		XtVaSetValues (push_button, 
			       XtVaTypedArg, XmNforeground, XmRString, text, strlen (text) + 1, NULL);
	
		XtFree (text); /* must free text gotten from XmStringUnparse() */
	}
}
