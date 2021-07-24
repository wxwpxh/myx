/* arrow.c -- demonstrate the ArrowButton widget.
** Have a Form widget display 4 ArrowButtons in a
** familiar arrangement.
*/

#include <Xm/ArrowBG.h>
#include <Xm/Form.h>

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, form, arrow;
	XrmDatabase  xrm_db;
	Arg          args[6];
	int          n;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos",  NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	xrm_db = XrmGetDatabase (XtDisplay (toplevel)) ;

	/* Rather than listing all these resources in an app-defaults file,
	** add them directly to the database for this application only. This
	** would be virtually equivalent to hard-coding values, since these
	** resources will override any other specified external to this file.
	*/
	
	XrmPutStringResource (&xrm_db, "*form*topAttachment", "attach_position");
	XrmPutStringResource (&xrm_db, "*form*leftAttachment", "attach_position");
	XrmPutStringResource (&xrm_db, "*form*rightAttachment", "attach_position");
	XrmPutStringResource (&xrm_db, "*form*bottomAttachment", "attach_position");
	
	n = 0;
	XtSetArg (args[n], XmNfractionBase, 3); n++;
	form = XmCreateForm (toplevel, "form",args, n);
	
	n = 0;
	XtSetArg (args[n], XmNtopPosition, 0); n++;
	XtSetArg (args[n], XmNbottomPosition, 1); n++;
	XtSetArg (args[n], XmNleftPosition, 1) ; n++;
	XtSetArg (args[n], XmNrightPosition, 2); n++;
	XtSetArg (args[n], XmNarrowDirection, XmARROW_UP); n++;
	arrow = XmCreateArrowButtonGadget (form, "arrow1", args, n);
	XtManageChild (arrow);

	n = 0;
	XtSetArg (args[n], XmNtopPosition, 1); n++;
	XtSetArg (args[n], XmNbottomPosition, 2); n++;
	XtSetArg (args[n], XmNleftPosition, 0) ; n++;
	XtSetArg (args[n], XmNrightPosition, 1); n++;
	XtSetArg (args[n], XmNarrowDirection, XmARROW_LEFT); n++;
	arrow = XmCreateArrowButtonGadget (form, "arrow2", args, n);
	XtManageChild (arrow);
	
	n = 0;
	XtSetArg (args[n], XmNtopPosition, 1); n++;
	XtSetArg (args[n], XmNbottomPosition, 2); n++;
	XtSetArg (args[n], XmNleftPosition, 2) ; n++;
	XtSetArg (args[n], XmNrightPosition, 3); n++;
	XtSetArg (args[n], XmNarrowDirection, XmARROW_RIGHT); n++;
	arrow = XmCreateArrowButtonGadget (form, "arrow3", args, n);
	XtManageChild (arrow);

	n = 0;
	XtSetArg (args[n], XmNtopPosition, 2); n++;
	XtSetArg (args[n], XmNbottomPosition, 3); n++;
	XtSetArg (args[n], XmNleftPosition, 1) ; n++;
	XtSetArg (args[n], XmNrightPosition, 2); n++;
	XtSetArg (args[n], XmNarrowDirection, XmARROW_DOWN); n++;
	arrow = XmCreateArrowButtonGadget (form, "arrow3", args, n);
	XtManageChild (arrow);

	XtManageChild (form);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
