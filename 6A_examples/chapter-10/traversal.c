/* traversal.c -- demonstrate keyboard traversal in a ScrolledWindow
** using the XmNtraverseObscuredCallback.
*/

#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/ScrolledW.h>
#include <Xm/RowColumn.h>

main (int argc, char *argv[])
{
	Widget       toplevel, sw, rc, child;
	XtAppContext app;
	void         traverse(Widget, XtPointer, XtPointer);
	int          i;
	char         name[10];
	Arg          args[4];

	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);
	
	i = 0;
	XtSetArg (args[i], XmNscrollingPolicy, XmAUTOMATIC); i++;
	sw = XmCreateScrolledWindow (toplevel, "scrolled_w", args, i);
	XtAddCallback (sw, XmNtraverseObscuredCallback, traverse, NULL);
	
	/* RowColumn is the work window for the widget */
	i = 0;
	XtSetArg (args[i], XmNorientation, XmHORIZONTAL); i++;
	XtSetArg (args[i], XmNpacking, XmPACK_COLUMN); i++;
	XtSetArg (args[i], XmNnumColumns, 10); i++;
	rc = XmCreateRowColumn (sw, "rc", args, i);

	for (i = 0; i < 10; i++) {
		sprintf (name, "Toggle %d", i);
		child = XmCreateToggleButton (rc, name, NULL, 0);
		XtManageChild (child);

		sprintf (name, "Button %d", i);
		child = XmCreatePushButton (rc, name, NULL, 0);
		XtManageChild (child);
	}

	XtManageChild (rc);
	XtManageChild (sw);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

void traverse (Widget widget, XtPointer client_data, XtPointer call_data)
{
	XmTraverseObscuredCallbackStruct *cbs = (XmTraverseObscuredCallbackStruct *) call_data;
	XmScrollVisible (widget, cbs->traversal_destination, 10, 10);
}
