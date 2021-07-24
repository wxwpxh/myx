/* paned_wind1.c -- two possible orientations of a PanedWindow.
** In the vertical PanedWindow, there are two Label widgets that are positioned 
** above and below a Text widget.  The Labels' minimum and maximum 
** sizes are set to 25 and 45 respectively, preventing those 
** panes from growing beyond those bounds.  The Text widget has its 
** minimum size set to 35 preventing it from becoming so small that 
** its text cannot be read. 
** In the horizontal orientation, the Label's minimum are set to 60
** to prevent the label from being truncated
*/

#include <Xm/Label.h>
#include <Xm/PanedW.h>
#include <Xm/Text.h>

Widget CreatePaneGroup (Widget parent, unsigned char orientation)
{
	Widget   pane, child;
	XmString xms;
	Arg      args[6];
	int      n = 0;
	
	XtSetArg (args[n], XmNorientation, orientation); n++;
	pane = XmCreatePanedWindow (parent, "pane", args, n);
	
	n = 0;
	if (orientation == XmVERTICAL) {
		XtSetArg (args[n], XmNpaneMinimum, 25); n++;
		XtSetArg (args[n], XmNpaneMaximum, 45); n++;
	}
	else {
		XtSetArg (args[n], XmNpaneMinimum, 60); n++;
	}
	child = XmCreateLabel (pane, "Hello", args, n);
	XtManageChild (child);
	
	n = 0;
	XtSetArg (args[n], XmNpaneMinimum, 35); n++;
	XtSetArg (args[n], XmNrows, 5); n++;
	XtSetArg (args[n], XmNcolumns, 80); n++;
	XtSetArg (args[n], XmNvalue, "This is a test of the PanedWindow widget"); n++;
	XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
	child = XmCreateText (pane, "text", args, n);
	XtManageChild (child);
	
	n = 0;
	if (orientation == XmVERTICAL) {
		XtSetArg (args[n], XmNpaneMinimum, 25); n++;
		XtSetArg (args[n], XmNpaneMaximum, 45); n++;
	}
	else {
		XtSetArg (args[n], XmNpaneMinimum, 60); n++;
	}

	child = XmCreateLabel (pane, "Goodbye", args, n);
	XtManageChild (child);
	
	return pane;
}

main (int argc, char *argv[])
{
	Widget       toplevel, pane;
	XtAppContext app;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);
					 
	pane = CreatePaneGroup (toplevel, (argc > 1 ? XmHORIZONTAL : XmVERTICAL));
	XtManageChild (pane);

	XtRealizeWidget (toplevel);
	XtAppMainLoop(app);
}
