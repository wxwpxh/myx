/* string_simplespin.c -- demonstrate the simple spin box widget */

#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/SSpinB.h>

char *months[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/*
** Callback which prints out, then increments,
** the SimpleSpinBox value
*/
void increment_spinbox (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget        spin = (Widget) client_data ;
	unsigned char type;
	int           position;

	XtVaGetValues (spin, XmNspinBoxChildType, &type, XmNposition, &position, NULL);

	printf ("type: %s current position: %d\n", 
		(type == XmNUMERIC ? "numeric" : "string"),
		position) ;

	if (type == XmNUMERIC) {
		int max;
		int min;

		XtVaGetValues (spin, XmNmaximumValue, &max, XmNminimumValue, &min, NULL);

		if (position == max) /* Wrap */
			position = min ;
		else
			position++;

		XtVaSetValues (spin, XmNposition, position, NULL);
	}
	else {
		XmStringTable values;
		int count;

		XtVaGetValues (spin, XmNvalues, &values,  XmNnumValues, &count, NULL);

		if (position == count - 1) /* Wrap */
			position = 0;
		else
			position++;

		XtVaSetValues (spin, XmNposition, position, NULL);
	}
}

main(int argc, char *argv[])
{
	Widget        toplevel, simple_b, push_b, rowcol, rc;
	XtAppContext  app;
	int           i, n = XtNumber (months);
	Arg           args[8];
	XmStringTable str_list;

	XtSetLanguageProc (NULL, NULL, NULL);

	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0,
		&argc, argv, NULL, sessionShellWidgetClass, NULL);

	rowcol = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);

	i = 0;
	XtSetArg (args[i], XmNorientation, XmHORIZONTAL); i++;
	rc = XmCreateRowColumn (rowcol, "rowcol", args, i);
	
	/* Create a numeric SimpleSpinBox */
	i = 0;
	XtSetArg (args[i], XmNmaximumValue, 31); i++;
	XtSetArg (args[i], XmNminimumValue, 1); i++;
	XtSetArg (args[i], XmNincrementValue, 1); i++;
	XtSetArg (args[i], XmNpositionType, XmPOSITION_VALUE); i++;
	XtSetArg (args[i], XmNposition, 1); i++;
	XtSetArg (args[i], XmNspinBoxChildType, XmNUMERIC); i++;
	simple_b = XmCreateSimpleSpinBox (rc, "simple", args, i);
	XtManageChild (simple_b);

	push_b = XmCreatePushButton (rc, "Push me", NULL, 0);
	XtAddCallback (push_b, XmNactivateCallback, increment_spinbox, (XtPointer) simple_b);
	XtManageChild (push_b) ;

	XtManageChild (rc);
	
	i = 0;
	XtSetArg (args[i], XmNorientation, XmHORIZONTAL); i++;
	rc = XmCreateRowColumn (rowcol, "rowcol", args, i);

	/* Create a string SimpleSpinBox */
	str_list = (XmStringTable) XtMalloc ((unsigned) n * sizeof (XmString *));

	for (i = 0; i < n; i++)
		str_list[i] = XmStringCreateLocalized (months[i]);

	i = 0;
	XtSetArg (args[i], XmNcolumns,	 3); i++;
	XtSetArg (args[i], XmNeditable,	 FALSE); i++;
	XtSetArg (args[i], XmNnumValues, n); i++;
	XtSetArg (args[i], XmNvalues,	str_list); i++;
	XtSetArg (args[i], XmNwrap,	TRUE); i++;
	XtSetArg (args[i], XmNpositionType, XmPOSITION_INDEX); i++;
	XtSetArg (args[i], XmNspinBoxChildType, XmSTRING); i++;
	simple_b = XmCreateSimpleSpinBox (rc, "simple", args, i);
	XtManageChild (simple_b);
	
	for (i = 0; i < n; i++)
		XmStringFree (str_list[i]);
	XtFree( (char *) str_list);
	
	push_b = XmCreatePushButton (rc, "Push me", NULL, 0);
	XtAddCallback (push_b, XmNactivateCallback, increment_spinbox, (XtPointer) simple_b);
	XtManageChild (push_b) ;
	
	XtManageChild (rc);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
