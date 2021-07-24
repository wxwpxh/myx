/* arrow_timer.c -- demonstrate continuous callbacks using
** ArrowButton widgets. Display up and down ArrowButtons and
** attach arm and disarm callbacks to them to start and stop timer
** that is called repeatedly while the button is down.  A label
** that has a value changes either positively or negatively
** by single increments while the button is depressed.
*/

#include <Xm/ArrowBG.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/LabelG.h>

XtAppContext app;
Widget       label;
XtIntervalId arrow_timer_id;

typedef struct value_range 
{
	int value, min, max;
} ValueRange;

main (int argc, char *argv[])
{
	Widget     w, toplevel, rowcol;
	void       start_stop(Widget, XtPointer, XtPointer);
	ValueRange range;
	Arg        args[6];
	int        n;
	XmString   xms;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	n = 0;
	XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
	rowcol = XmCreateRowColumn (toplevel, "rowcol", args, n);

	n = 0;
	XtSetArg (args[n], XmNarrowDirection, XmARROW_UP); n++;
	w = XmCreateArrowButtonGadget (rowcol, "arrow_up", args, n);
	XtAddCallback (w, XmNarmCallback, start_stop, (XtPointer) 1);
	XtAddCallback (w, XmNdisarmCallback, start_stop, (XtPointer) 1);
	XtManageChild (w);

	n = 0;
	XtSetArg (args[n], XmNarrowDirection, XmARROW_DOWN); n++;
	w = XmCreateArrowButtonGadget (rowcol, "arrow_dn", args, n);
	XtAddCallback (w, XmNarmCallback, start_stop, (XtPointer) -1);
	XtAddCallback (w, XmNdisarmCallback, start_stop, (XtPointer) -1);
	XtManageChild (w);

	range.value = 0;
	range.min = -50;
	range.max = 50;

	n = 0;
	xms = XmStringCreateLocalized("3");
	XtSetArg (args[n], XmNlabelString, xms); n++;
	XtSetArg (args[n], XmNuserData, &range); n++;
	label = XmCreateLabelGadget (rowcol, "label", args, n);
	XmStringFree(xms);
	XtManageChild (label);

	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* start_stop is used to start or stop the incremental changes to
** the label's value.  When the button goes down, the reason is
** XmCR_ARM and the timer starts.  XmCR_DISARM disables the timer.
*/

void start_stop (Widget w, XtPointer client_data, XtPointer call_data)
{
	int                          incr = (int) client_data;
	XmArrowButtonCallbackStruct *cbs  = (XmArrowButtonCallbackStruct *) call_data;
	void                         change_value(XtPointer, XtIntervalId *);

	if (cbs->reason == XmCR_ARM)
		change_value ((XtPointer) incr, (XtIntervalId *) 1 );
	else if (cbs->reason == XmCR_DISARM)
		XtRemoveTimeOut (arrow_timer_id);
}

/* change_value is called each time the timer expires.  This function
** is also used to initiate the timer.  The "id" represents that timer
** ID returned from the last call to XtAppAddTimeOut().  If id == 1,
** the function was called from start_stop(), not a timeout.  If the value
** has reached its maximum or minimum, don't restart timer, just return.
** If id == 1, this is the first timeout so make it be longer to allow
** the user to release the button and avoid getting into the "speedy"
** part of the timeouts.
*/
void change_value (XtPointer client_data,  XtIntervalId *id)
{
	ValueRange *range;
	char        buf[8];
	int         incr = (int) client_data;
	XmString    xms;

	XtVaGetValues (label, XmNuserData, &range, NULL);

	if (range->value + incr > range->max || range->value + incr < range->min)
		return;

	range->value += incr;

	(void) sprintf (buf, "%d", range->value);
	xms = XmStringCreateLocalized (buf);

	XtVaSetValues (label, XmNlabelString, xms, NULL);
	XmStringFree(xms);

	arrow_timer_id = XtAppAddTimeOut (app, 
					  (unsigned long) (id == (XtIntervalId *) 1 ? 500 : 100), 
					  change_value, 
					  (XtPointer) incr);
}
