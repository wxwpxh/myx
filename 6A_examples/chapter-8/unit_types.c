/* unit_types.c --the same as paned_win1.c except that the
** Labels' minimum and maximum sizes are set to 1/4 inch and
** 1/2 inch respectively.  These measurements are retained
** regardless of the pixels-per-inch resolution of the user's
** display.
*/

#include <Xm/Label.h>
#include <Xm/PanedW.h>
#include <Xm/Text.h>

main (int argc, char *argv[])
{
	Widget       toplevel, pane, child;
	XtAppContext app;
	Arg          args[6];
	int          n; 
	
	XtSetLanguageProc (NULL, NULL, NULL);

	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	n = 0;
	XtSetArg (args[n], XmNunitType, Xm1000TH_INCHES); n++;
	pane = XmCreatePanedWindow (toplevel, "pane", args, n);
	
	n= 0;
	XtSetArg (args[n], XmNpaneMinimum, 250); n++; /* quarter inch */
	XtSetArg (args[n], XmNpaneMaximum, 500); n++; /* half inch */
	child = XmCreateLabel (pane, "Hello",args, n);
	XtManageChild (child);

	n = 0;
	XtSetArg (args[n], XmNrows, 5); n++;
	XtSetArg (args[n], XmNcolumns, 80); n++;
	XtSetArg (args[n], XmNpaneMinimum, 250); n++; /* quarter inch */
	XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
	XtSetArg (args[n], XmNvalue, "This is a test of the paned window widget."); n++;
	child = XmCreateText (pane, "text", args, n);
	XtManageChild (child);

	n = 0;
	XtSetArg (args[n], XmNpaneMinimum, 250); n++; /* quarter inch */
	XtSetArg (args[n], XmNpaneMaximum, 500); n++; /* half inch */
	child = XmCreateLabel (pane, "Goodbye", args, n);
	XtManageChild (child);

	XtManageChild (pane);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
