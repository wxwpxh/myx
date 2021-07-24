/* simple_scale.c -- demonstrate a few scale widgets. */

#include <Xm/Scale.h>
#include <Xm/RowColumn.h>

Widget create_scale (Widget parent, char *title, int max, int min, int value)
{
	Arg      args[8];
	int      n = 0;
	XmString xms = XmStringCreateLocalized (title);
	void     new_value(Widget, XtPointer, XtPointer); /* callback for Scale widgets */
	Widget   scale;

	XtSetArg (args[n], XmNtitleString, xms); n++;
	XtSetArg (args[n], XmNmaximum, max); n++;
	XtSetArg (args[n], XmNminimum, min); n++;
	XtSetArg (args[n], XmNvalue, value); n++;
	XtSetArg (args[n], XmNshowValue, True); n++;

	scale = XmCreateScale (parent, title, args, n);
	XtAddCallback (scale, XmNvalueChangedCallback, new_value, NULL);
	XtManageChild (scale);

	return scale;
}

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, scale;
	Arg          args[2];
	XtAppContext app;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	
	XtSetArg (args[0], XmNorientation, XmHORIZONTAL); 
	rowcol = XmCreateRowColumn (toplevel, "rowcol", args, 1);
	
	scale = create_scale (rowcol, "Days", 7, 1, 1);
	scale = create_scale (rowcol, "Weeks", 52, 1, 1);
	scale = create_scale (rowcol, "Months", 12, 1, 1);
	scale = create_scale (rowcol, "Years", 20, 1, 1);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
void new_value (Widget scale_w, XtPointer client_data, XtPointer call_data)
{
	XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *) call_data;
	
	printf("%s: %d\n", XtName(scale_w), cbs->value);
}
