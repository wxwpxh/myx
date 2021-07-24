/* proc_traverse.c -- demonstrate how to process keyboard traversal
** from a PushButton's callback routine.  This simple demo contains
** a RowColumn (a tab group) and three PushButtons.  If any of the
** PushButtons are activated (selected), the input focus traverses
** to the "home" item.
*/ 

#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, pb;
	XtAppContext app;
	Arg          args[2];
	void         do_it(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0,  &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	XtSetArg (args[0], XmNorientation, XmHORIZONTAL); 
	rowcol = XmCreateRowColumn (toplevel, "rowcolumn",  args, 1); 
	
	pb = XmCreatePushButton (rowcol, "OK", NULL, 0);
	XtManageChild (pb);
	
	pb = XmCreatePushButton (rowcol, "Cancel", NULL, 0);
	XtAddCallback (pb, XmNactivateCallback, do_it, NULL);
	XtManageChild (pb);
	
	pb = XmCreatePushButton (rowcol, "Help", NULL, 0);
	XtAddCallback (pb, XmNactivateCallback, do_it, NULL);
	XtManageChild (pb);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* callback for pushbuttons */
void do_it (Widget widget, XtPointer client_data, XtPointer call_data)
{
	/* do stuff here for PushButton widget */
	XmProcessTraversal(widget, XmTRAVERSE_HOME);
}
