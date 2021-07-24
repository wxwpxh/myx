/* numeric_simplespin.c -- demonstrate the simple spin box widget */

#include <Xm/Xm.h>
#include <Xm/SSpinB.h>

main(int argc, char *argv[])
{
	Widget        toplevel, simple_b;
	XtAppContext  app;
	int           n;
	Arg           args[8];
	XmStringTable str_list;

	XtSetLanguageProc (NULL, NULL, NULL);

	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0,
		&argc, argv, NULL, sessionShellWidgetClass, NULL);

	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmNUMERIC); n++;
	XtSetArg (args[n], XmNminimumValue, 0); n++;
	XtSetArg (args[n], XmNmaximumValue, 20); n++;
	XtSetArg (args[n], XmNincrementValue, 2); n++;
	XtSetArg (args[n], XmNeditable, TRUE); n++;
	XtSetArg (args[n], XmNcolumns, 10); n++;
	XtSetArg (args[n], XmNwrap, FALSE); n++;
	simple_b = XmCreateSimpleSpinBox (toplevel, "simple", args, n);
	
	XtManageChild (simple_b);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
