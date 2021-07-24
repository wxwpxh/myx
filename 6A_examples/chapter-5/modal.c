/* modal.c -- demonstrate modal dialogs. Display two pushbuttons 
** each activating a modal dialog.
*/

#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

/* main() --create a pushbutton whose callback pops up a dialog box */
main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, button, rowcolumn;
	void         pushed(Widget, XtPointer, XtPointer); 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);
					 
	rowcolumn = XmCreateRowColumn (toplevel, "rowcolumn", NULL, 0);

	button = XmCreatePushButton (rowcolumn, "Application Modal", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, pushed, (XtPointer) XmDIALOG_FULL_APPLICATION_MODAL);
	XtManageChild (button);
	
	button = XmCreatePushButton (rowcolumn, "System Modal", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, pushed, (XtPointer) XmDIALOG_SYSTEM_MODAL);
	XtManageChild (button);
	
	XtManageChild (rowcolumn);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* pushed() --the callback routine for the main app's pushbutton.
** Create either a full-application or system modal dialog box.
*/
void pushed (Widget widget, XtPointer client_data, XtPointer call_data)
{
	static Widget dialog = (Widget) 0 ;
	XmString      t;
	void          dlg_callback(Widget, XtPointer, XtPointer);
	unsigned char modality = (unsigned char) client_data;
	
	/* See if we've already created this dialog -- if so,
	** we don't need to create it again. Just re-pop it up.
	*/
	
	if (!dialog) {
		Arg args[5];
		int n = 0;
		XmString ok = XmStringCreateLocalized ("OK");

		XtSetArg(args[n], XmNautoUnmanage, False); n++;
		XtSetArg(args[n], XmNcancelLabelString, ok); n++;
		dialog = XmCreateInformationDialog (widget, "notice", args, n);
		XtAddCallback (dialog, XmNcancelCallback, dlg_callback, NULL);

		XtUnmanageChild (XtNameToWidget (dialog, "OK"));
		XtUnmanageChild (XtNameToWidget (dialog, "Help"));
	}
	
	t = XmStringCreateLocalized ("You must reply to this message now!"); 
	XtVaSetValues (dialog, XmNmessageString, t, XmNdialogStyle, modality, NULL);
	XmStringFree (t);
	XtManageChild (dialog);
}

void dlg_callback (Widget dialog, XtPointer client_data, XtPointer call_data)
{
	XtUnmanageChild (dialog);
}
