/* string_simplespin.c -- demonstrate the simple spin box widget */

#include <Xm/Xm.h>
#include <Xm/SSpinB.h>

char *months[] = {
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
};

main(int argc, char *argv[])
{
	Widget        toplevel, simple_b;
	XtAppContext  app;
	int           i, n = XtNumber (months);
	Arg           args[8];
	XmStringTable str_list;

	XtSetLanguageProc (NULL, NULL, NULL);

	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0,
		&argc, argv, NULL, sessionShellWidgetClass, NULL);

	str_list = (XmStringTable) XtMalloc ((unsigned) n * sizeof (XmString *));

	for (i = 0; i < n; i++)
		str_list[i] = XmStringCreateLocalized (months[i]);

	i = 0;
	XtSetArg (args[i], XmNspinBoxChildType,	 XmSTRING); i++;
	XtSetArg (args[i], XmNeditable,	 FALSE); i++;
	XtSetArg (args[i], XmNnumValues, n); i++;
	XtSetArg (args[i], XmNvalues,	str_list); i++;
	XtSetArg (args[i], XmNwrap,	TRUE); i++;

	simple_b = XmCreateSimpleSpinBox (toplevel, "simple", args, i);
	
	for (i = 0; i < n; i++)
		XmStringFree (str_list[i]);
	XtFree( (XtPointer) str_list);
	
	XtManageChild (simple_b);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
