/* wm_delete.c -- demonstrate how to bind the Close button in the
** window manager's system menu to the "cancel" button in a dialog.
*/

#include <Xm/MessageB.h>
#include <Xm/PushB.h>
#include <Xm/Protocols.h>

main (int argc, char *argv[])
{
	Widget       toplevel, button;
	XtAppContext app;
	void         activate(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	button = XmCreatePushButton (toplevel, "Push Me", NULL, 0);
	XtManageChild (button);
	XtAddCallback (button, XmNactivateCallback, activate, NULL);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* Create and popup an ErrorDialog indicating that the user may have
** done something wrong. The dialog contains an OK and Cancel button,
** but he can still choose the Close button in the titlebar.
*/
void activate (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget   dialog, shell;
	void     response(Widget, XtPointer, XtPointer);
	XmString t = XmStringCreateLocalized ("Warning: Delete All Files?");
	Atom     WM_DELETE_WINDOW;
	Arg      args[5];
	int      n;

	/* Make sure the VendorShell associated with the dialog does not
	** react to the user's selection of the Close system menu item.
	*/
	n = 0;
	XtSetArg (args[n], XmNmessageString, t); n++;
	XtSetArg (args[n], XmNdeleteResponse, XmDO_NOTHING); n++;
	dialog = XmCreateWarningDialog (w, "notice", args, n);
	XmStringFree (t);
	/* add callback routines for ok and cancel -- desensitize help */
	XtAddCallback (dialog, XmNokCallback, response, NULL);
	XtAddCallback (dialog, XmNcancelCallback, response, NULL);
	XtSetSensitive (XtNameToWidget (dialog, "Help"), False);
	XtManageChild (dialog);
	/* Add a callback for the WM_DELETE_WINDOW protocol */
	shell = XtParent (dialog);
	WM_DELETE_WINDOW = XInternAtom (XtDisplay (w), "WM_DELETE_WINDOW", False);
	XmAddWMProtocolCallback (shell, WM_DELETE_WINDOW, response, (XtPointer) dialog);
}

/* callback for the OK and Cancel buttons in the dialog -- may also be
** called from the WM_DELETE_WINDOW protocol message sent by the wm.
*/
void response (Widget widget, XtPointer client_data, XtPointer call_data)
{
	XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;
	Widget               dialog;
	
	if (cbs->reason == XmCR_OK)
		puts ("Yes");
	else
		puts ("No");
		
	if (cbs->reason == XmCR_PROTOCOLS)
		/* we passed the dialog as client data for the protocol callback */
		dialog = (Widget) client_data;
	else
		dialog = widget;
		
	XtDestroyWidget (dialog);
}
