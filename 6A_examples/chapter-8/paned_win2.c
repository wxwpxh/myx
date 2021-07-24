/* paned_wind2.c --there are two label widgets that are positioned
** above and below a Text widget.  The labels' desired heights are
** queried using XtQueryGeometry() and their corresponding maximum
** and minimum sizes are set to the same value.  This effectively
** prevents those panes from being resized.  The Text widget has its
** minimum size set to 35 preventing it from becoming so small that
** its text cannot be read.
*/

#include <Xm/Label.h>
#include <Xm/PanedW.h>
#include <Xm/Text.h>

main (int argc, char *argv[])
{
	Widget           toplevel, pane, label, text;
	XtWidgetGeometry size; 
	XtAppContext     app;
	Arg              args[8];
	int              n;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
                                         sessionShellWidgetClass, NULL);

	pane = XmCreatePanedWindow (toplevel, "pane", NULL, 0);
	
	label = XmCreateLabel (pane, "Hello",  NULL, 0);
	XtManageChild (label);
	
	size.request_mode = CWHeight;
	XtQueryGeometry (label, NULL, &size);
	XtVaSetValues (label, XmNpaneMaximum, size.height, XmNpaneMinimum, size.height, NULL);
	
	printf ("hello's height: %d\n", size.height);
	
	n = 0;
	XtSetArg (args[n], XmNrows, 5); n++;
	XtSetArg (args[n], XmNcolumns, 80); n++;
	XtSetArg (args[n], XmNresizeWidth, False); n++;
	XtSetArg (args[n], XmNresizeHeight, False); n++;
	XtSetArg (args[n], XmNpaneMinimum, 35); n++;
	XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
	XtSetArg (args[n], XmNvalue,  "This is a test of the paned window widget."); n++;
	text = XmCreateText (pane, "text", args, n);
	XtManageChild (text);
	
	label = XmCreateLabel (pane, "Goodbye", NULL, 0);
	XtManageChild (label);
	
	size.request_mode = CWHeight;
	XtQueryGeometry (label, NULL, &size);
	XtVaSetValues (label, XmNpaneMaximum, size.height,  XmNpaneMinimum, size.height, NULL);
	
	printf ("goodbye's height: %d\n", size.height);
	
	XtManageChild (pane);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
