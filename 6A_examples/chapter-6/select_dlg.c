/* select_dlg.c -- display two pushbuttons: days and months.
** When the user selections one of them, post a selection
** dialog that displays the actual days or months accordingly.
** When the user selects or types a selection, post a dialog
** the identifies which item was selected and whether or not
** the item is in the list.
**
** This program demonstrates how to use selection boxes,
** methods for creating generic callbacks for action area
** selections, abstraction of data structures, and a generic
** MessageDialog posting routine.
*/

#include <Xm/SelectioB.h>
#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

Widget PostDialog();
char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char *months[] = {"January", "February", "March", "April", "May", "June", 
                  "July", "August", "September", "October", "November", "December"};

typedef struct 
{
	char  *label;
	char **strings;
	int    size;
} ListItem;

ListItem month_items = {"Months", months, XtNumber (months)};
ListItem days_items = {"Days", days, XtNumber (days)};

/* main() --create two pushbuttons whose callbacks pop up a dialog */
main (int argc, char *argv[])
{
	Widget       toplevel, button, rc; 
	XtAppContext app; 
	void         pushed(Widget, XtPointer, XtPointer); 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);   

	rc = XmCreateRowColumn (toplevel, "rowcolumn", NULL, 0);
	button = XmCreatePushButton (rc, month_items.label, NULL, 0);
	XtAddCallback (button, XmNactivateCallback, pushed, (XtPointer) &month_items);
	XtManageChild (button);

	button = XmCreatePushButton (rc, days_items.label, NULL, 0);
	XtAddCallback (button, XmNactivateCallback, pushed, (XtPointer) &days_items);
	XtManageChild (button);
	
	XtManageChild (rc);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* pushed() --the callback routine for the main app's pushbutton.
** Create a dialog containing the list in the items parameter.
*/
void pushed (Widget widget, XtPointer client_data, XtPointer call_data)
{
	Widget    dialog;
	XmString  t, *str;
	int       i;
	void      dialog_callback(Widget, XtPointer, XtPointer);
	ListItem *items = (ListItem *) client_data;
	
	str = (XmString *) XtMalloc (items->size * sizeof (XmString));
	t = XmStringCreateLocalized (items->label);
	
	for (i = 0; i < items->size; i++)
		str[i] = XmStringCreateLocalized (items->strings[i]);

	dialog = XmCreateSelectionDialog (widget, "selection", NULL, 0);
	
	XtVaSetValues (dialog, XmNlistLabelString, t, 
			       XmNlistItems, str,
			       XmNlistItemCount, items->size,
			       XmNmustMatch, True,
			       NULL);   
			       
	XtSetSensitive (XtNameToWidget (dialog, "Help"), False);
	
	XtAddCallback (dialog, XmNokCallback, dialog_callback, NULL);
	XtAddCallback (dialog, XmNnoMatchCallback, dialog_callback, NULL);
	XmStringFree (t);
	
	while (--i >= 0)
		XmStringFree (str[i]); /* free elements of array */

	XtFree ((char *) str); /* now free array pointer */

	XtManageChild (dialog);
}

/* dialog_callback() --The OK button was selected or the user 
** input a name by himself. Determine whether the result is 
** a valid name by looking at the "reason" field. 
*/
void dialog_callback (Widget widget, XtPointer client_data, XtPointer call_data)
{
	char msg[256], *prompt, *value;
	int  dialog_type;
	XmSelectionBoxCallbackStruct *cbs = (XmSelectionBoxCallbackStruct *) call_data;
	
	switch (cbs->reason) {
		case XmCR_OK : prompt = "Selection: "; dialog_type = XmDIALOG_MESSAGE; break; 
		case XmCR_NO_MATCH : prompt = "Not a valid selection: "; dialog_type = XmDIALOG_ERROR; break;
		default : prompt = "Unknown selection: "; dialog_type = XmDIALOG_ERROR; break;
	}
	
	value = (char *) XmStringUnparse (cbs->value, XmFONTLIST_DEFAULT_TAG, 
					  XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL);
					  
	sprintf (msg, "%s%s", prompt, value);
	XtFree (value);
	
	(void) PostDialog (XtParent (XtParent (widget)), dialog_type, msg);
	
	if (cbs->reason != XmCR_NO_MATCH) {
		XtUnmanageChild (widget);
		XtDestroyWidget (XtParent (widget)); /* The shell parent of the Selection box */
	}
}

/*
** Destroy the shell parent of the Message box, and thus the box itself
*/
void destroy_dialog (Widget dialog, XtPointer client_data, XtPointer call_data)
{
	XtDestroyWidget (XtParent (dialog)); /* The shell parent of the Message box */
}

/*
** PostDialog() -- a generalized routine that allows the programmer
** to specify a dialog type (message, information, error, help, * etc..), and the message to show.
*/
Widget PostDialog (Widget parent, int dialog_type, char *msg)
{
	Widget   dialog;
	XmString text;
	
	dialog = XmCreateMessageDialog (parent, "dialog", NULL, 0);
	text = XmStringCreateLocalized (msg);
	XtVaSetValues (dialog, XmNdialogType, dialog_type, XmNmessageString, text, NULL);
	XmStringFree (text);

	XtUnmanageChild (XtNameToWidget (dialog, "Cancel"));
	XtSetSensitive (XtNameToWidget (dialog, "Help"), False);
	XtAddCallback (dialog, XmNokCallback, destroy_dialog, NULL);

	XtManageChild (dialog);
	
	return dialog;
}
