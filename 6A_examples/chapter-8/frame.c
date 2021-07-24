/* frame.c -- demonstrate the Frame widget by creating
** four Labels with Frame widget parents.
*/

#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/Frame.h>

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, frame, label;
	XtAppContext app;
	Arg          args[10];
	int          n; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	/* Initialize toolkit and create TopLevel shell widget */
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	/* Make a RowColumn to contain all the Frames */
	n = 0;
	XtSetArg (args[n], XmNspacing, 5); n++;
	rowcol = XmCreateRowColumn (toplevel, "rowcolumn",  args, n);
	
	/* Some informative labelling */
	label = XmCreateLabelGadget (rowcol, "Frame Types:", NULL, 0);
	XtManageChild (label);
	
	/* Create different Frames each containing a unique shadow type */
	/* First frame: Shadow in */
	n = 0;
	XtSetArg (args[n], XmNshadowType, XmSHADOW_IN); n++;
	frame = XmCreateFrame (rowcol, "frame1",   args, n);
	label = XmCreateLabelGadget (frame, "XmSHADOW_IN", NULL, 0);
	XtManageChild (label);
	
	n = 0;
	XtSetArg (args[n], XmNframeChildType, XmFRAME_TITLE_CHILD); n++;
	XtSetArg (args[n], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); n++;
	label = XmCreateLabelGadget (frame, "XmALIGNMENT_CENTER", args, n);
	XtManageChild (label);
	XtManageChild (frame);
	
	/* Second frame: Shadow out */
	n = 0;
	XtSetArg (args[n], XmNshadowType, XmSHADOW_OUT); n++;
	frame = XmCreateFrame (rowcol, "frame2",  args, n);
	label = XmCreateLabelGadget (frame, "XmSHADOW_OUT", NULL, 0);
	XtManageChild (label);
	
	n = 0;
	XtSetArg (args[n], XmNframeChildType, XmFRAME_TITLE_CHILD); n++;
	XtSetArg (args[n], XmNchildVerticalAlignment, XmALIGNMENT_BASELINE_TOP); n++;
	label = XmCreateLabelGadget (frame, "XmALIGNMENT_BASELINE_TOP", args, n);
	XtManageChild (label);
	XtManageChild (frame);
	
	/* Third frame: Etched in */
	n = 0;
	XtSetArg (args[n], XmNshadowType, XmSHADOW_ETCHED_IN); n++;
	frame = XmCreateFrame (rowcol, "frame3", args, n);
	label = XmCreateLabelGadget (frame, "XmSHADOW_ETCHED_IN",  NULL, 0);
	XtManageChild (label);
	
	n = 0;
	XtSetArg (args[n], XmNframeChildType, XmFRAME_TITLE_CHILD); n++;
	XtSetArg (args[n], XmNchildVerticalAlignment, XmALIGNMENT_WIDGET_TOP); n++;
	label = XmCreateLabelGadget (frame, "XmALIGNMENT_WIDGET_TOP",         args, n);
	XtManageChild (label);
	XtManageChild (frame);
	
	/* Fourth frame: Etched out */
	n = 0;
	XtSetArg (args[n], XmNshadowType, XmSHADOW_ETCHED_OUT); n++;
	frame = XmCreateFrame (rowcol, "frame4",  args, n);
	label = XmCreateLabelGadget (frame, "XmSHADOW_ETCHED_OUT", NULL, 0);
	XtManageChild (label);

	n = 0;
	XtSetArg (args[n], XmNframeChildType, XmFRAME_TITLE_CHILD); n++;
	XtSetArg (args[n], XmNchildVerticalAlignment, XmALIGNMENT_WIDGET_BOTTOM); n++;
	label = XmCreateLabelGadget (frame, "XmALIGNMENT_WIDGET_BOTTOM",  args, n);
	XtManageChild (label);
	XtManageChild (frame);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
