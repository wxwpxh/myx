/* simple_sb.c -- demonstrate the Scrollbar resource values from
** a ScrolledText object. This is used as an introductory examination
** of the resources used by Scrollbars.
*/
#include <Xm/ScrolledW.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>
#include <Xm/Text.h>

/* print the "interesting" resource values of a scrollbar */
void get_sb (Widget widget, XtPointer client_data, XtPointer call_data)
{
	Widget scrollbar = (Widget) client_data;
	int increment=0, maximum=0, minimum=0;
	int page_incr=0, slider_size=0, value=0;
	
	XtVaGetValues (scrollbar, XmNincrement, &increment,
				  XmNmaximum, &maximum,
				  XmNminimum, &minimum,
				  XmNpageIncrement, &page_incr,
				  XmNsliderSize, &slider_size,
				  XmNvalue, &value,
				  NULL);    

	printf ("increment=%d, max=%d, min=%d, page=%d, slider=%d, value=%d\n",
		increment, maximum, minimum, page_incr, slider_size, value);
}

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, text_w, pb, sb;
	XtAppContext app;
	Arg          args[10];
	int          n = 0;

	XtSetLanguageProc (NULL, NULL, NULL);

	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
	                                 sessionShellWidgetClass, NULL);

	/* RowColumn contains ScrolledText and PushButton */
	rowcol = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);

	XtSetArg (args[n], XmNrows, 10); n++;
	XtSetArg (args[n], XmNcolumns, 80); n++;
	XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
	XtSetArg (args[n], XmNscrollHorizontal, False); n++;
	XtSetArg (args[n], XmNwordWrap, True); n++;
	text_w = XmCreateScrolledText (rowcol, "text_w", args, n);
	XtManageChild (text_w);

	/* get the scrollbar from ScrolledWindow associated with Text widget */
	XtVaGetValues (XtParent (text_w), XmNverticalScrollBar, &sb, NULL);

	/* provide a pushbutton to obtain the scrollbar's resource values */
	pb = XmCreatePushButtonGadget (rowcol, "Print ScrollBar Values", NULL, 0);
	XtAddCallback (pb, XmNactivateCallback, get_sb, sb);
	XtManageChild (pb);

	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
