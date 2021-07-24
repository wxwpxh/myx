/* utm.c: transfers data between a specimen SpinBox and
** a TextField using the Uniform Transfer Model 
*/

#include <X11/Intrinsic.h>
#include <Xm/Transfer.h>
#include <Xm/RowColumn.h>
#include <Xm/SpinB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>

void GetDate (Widget text, short *day, short *month, short *year)
{
	int d, m, y;
	Widget spinb = XtParent (text) ;

	XtVaGetValues (XtNameToWidget (spinb, "days"),   XmNposition, &d, 0) ;
	XtVaGetValues (XtNameToWidget (spinb, "months"), XmNposition, &m, 0) ;
	XtVaGetValues (XtNameToWidget (spinb, "years"),  XmNposition, &y, 0) ;

	*day   = (short) d ;
	*month = (short) m ;
	*year  = (short) y ;
}

void convert_callback (Widget widget, XtPointer client_data, XtPointer call_data)
{
	XmConvertCallbackStruct *cptr = (XmConvertCallbackStruct *) call_data;
	Atom     TARGETS, EXPORTS, CB_TARGETS, DATE_TARGET;
	Atom    *targets;
	Display *display = XtDisplay (widget);
	
	/* Intern the atoms */
	TARGETS     = XInternAtom (display, "TARGETS", False);
	EXPORTS     = XInternAtom (display, "_MOTIF_EXPORT_TARGETS", False);
	CB_TARGETS  = XInternAtom (display, "_MOTIF_CLIPBOARD_TARGETS", False);
	DATE_TARGET = XInternAtom (display, "APPLICATION_DATE", False);

	/* If the destination has requested the list of targets */
	/* we return this as the convert data */
	if ((cptr->target == TARGETS) || (cptr->target == CB_TARGETS) || (cptr->target == EXPORTS)) {
		/* A request from the destination for the supported */
		/* data types we are willing to handle */

		targets      = (Atom *) XtMalloc ((unsigned) sizeof (Atom));
		targets[0]   = DATE_TARGET;
		cptr->type   = XA_ATOM;
		cptr->value  = (XtPointer) targets;
		cptr->length = 1;
		cptr->format = 32;
		cptr->status = XmCONVERT_MERGE;
	}
	else {
		/* A request from the destination for a specific data type */
		if (cptr->target == DATE_TARGET) {
			/* An unspecified routine to fetch the date */
			/* Presumably the current widget is a date field */
			/* So we pass the widget parameter to fetch this */
			short *date = (short *) XtMalloc ((unsigned) 3 * sizeof (short));

			GetDate (widget, &date[0], &date[1], &date[2]);
			
			cptr->value  = (XtPointer) date;
			cptr->length = 3;
			cptr->type   = cptr->target; /* As requested */
			cptr->format = 16; /* 8 = char, 16 = short, 32 = long */
			cptr->status = XmCONVERT_DONE;
		}
		else {
			cptr->value   = (XtPointer) 0;
			cptr->length  = 0;
			cptr->format  = 0;
			cptr->type    = cptr->target;
			cptr->status  = XmCONVERT_MERGE;
		}
	}
}


void transfer_callback ( Widget w, /* The destination widget */
			 XtPointer client_data,
			 XtPointer call_data)
{
	XmSelectionCallbackStruct *sptr = (XmSelectionCallbackStruct *) call_data;
	Atom     EXPORTS, TARGETS, CB_TARGETS, DATE_TARGET;
	Display *display = XtDisplay (w);
	Atom    *targets, choice;
	int      i;
	
	choice      = (Atom) 0;
	TARGETS     = XInternAtom (display, "TARGETS", False);
	EXPORTS     = XInternAtom (display, "_MOTIF_EXPORT_TARGETS", False);
	CB_TARGETS  = XInternAtom (display, "_MOTIF_CLIPBOARD_TARGETS", False);
	DATE_TARGET = XInternAtom (display, "APPLICATION_DATE", False);
	
	if ((sptr->type == XA_ATOM) && 
	    ((sptr->target == TARGETS) || (sptr->target == CB_TARGETS) || (sptr->target == EXPORTS))) {
		/* The source has sent us a list of available data formats */
		/* in which it is prepared to export the data */
		/* We get to choose one */
		/* The value field contains the list of available targets */
		targets = (Atom *) sptr->value;
		
		for (i = 0 ; i < sptr->length ; i++) {
			if (targets[i] == DATE_TARGET) {
				/* We like this: its our own preferred format */
				choice = targets[i];
			}
		}
		
		/* If the source is not prepared to export in a format */
		/* of our choice, we can either let the toolkit handle it */
		/* under the assumption its a built-in data type */
		/* or we can signal that the transfer is no good */
		
		/* Lets assume we are only interested in our own data transfer */
		if (choice == (Atom) 0) {
			XmTransferDone (sptr->transfer_id, XmTRANSFER_DONE_FAIL);
			return;
		}
		
		/* On the other hand, if we have chosen a target */
		/* We simply go back to the source asking for it */
		XmTransferValue (sptr->transfer_id, 
				 choice,
				 transfer_callback, /* Round we go again */
				 NULL,
				 XtLastTimestampProcessed(display));
	 }
	 else {
		/* The source has sent us a specific data format */
		/* It **ought** to be DATE_TARGET, but better check... */
		if (sptr->target == DATE_TARGET) {
			/* Three integers, we assume */
			/* We really ought to check sptr->length */
			short *date = (short *) sptr->value; 
			char buf[32];
			
			/* Not pretty, but it will do for here */
			(void) sprintf (buf, "%d/%d/%d", date[0], date[1], date[2]);
			XmTextFieldSetString (w, buf);
			
			XmTransferDone (sptr->transfer_id, XmTRANSFER_DONE_SUCCEED);
		}
		else {
			/* We should not be here. Someone has written a */
			/* convert procedure which is out of step. */
			
			XmTransferDone (sptr->transfer_id, XmTRANSFER_DONE_FAIL);
		}
	}
}

void destination_callback ( Widget w, XtPointer client_data, XtPointer call_data)
{
	XmDestinationCallbackStruct *dptr = (XmDestinationCallbackStruct *) call_data;
	
	Atom TARGETS = XInternAtom (XtDisplay (w), "TARGETS", False);
	
	XmTransferValue ( dptr->transfer_id, 
			  TARGETS, 
			  transfer_callback, 
			  NULL, 
			  XtLastTimestampProcessed(XtDisplay (w)));
}

main(int argc, char *argv[])
{
	Widget        toplevel, spin, rowcol;
	Widget        day, month, year, text;
	XtAppContext  app;
	Arg           args[8];
	int           n;

	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);

	rowcol = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);

	/* Create the SpinBox */
	spin = XmCreateSpinBox (rowcol, "spin", NULL, 0);
	
	/* Create the Days field */
	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmNUMERIC) ; n++;
	XtSetArg (args[n], XmNcolumns, 2) ; n++;
	XtSetArg (args[n], XmNeditable, FALSE) ; n++;
	XtSetArg (args[n], XmNminimumValue, 1) ; n++;
	XtSetArg (args[n], XmNmaximumValue, 31) ; n++;
	XtSetArg (args[n], XmNposition, 1) ; n++;
	XtSetArg (args[n], XmNwrap, TRUE) ; n++;
	
	day = XmCreateTextField (spin, "days", args, n);
	XtManageChild (day);

	/* Create the Months field */
	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmNUMERIC); n++;
	XtSetArg (args[n], XmNcolumns, 2); n++;
	XtSetArg (args[n], XmNeditable, FALSE); n++;
	XtSetArg (args[n], XmNminimumValue, 1); n++;
	XtSetArg (args[n], XmNmaximumValue, 12); n++;
	XtSetArg (args[n], XmNposition, 1) ; n++;
	XtSetArg (args[n], XmNwrap, TRUE); n++;
	month = XmCreateTextField (spin, "months", args, n);
	XtManageChild (month);

	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmNUMERIC); n++;
	XtSetArg (args[n], XmNcolumns, 4); n++;
	XtSetArg (args[n], XmNeditable, FALSE); n++;
	XtSetArg (args[n], XmNminimumValue, 1900); n++;
	XtSetArg (args[n], XmNmaximumValue, 2100); n++;
	XtSetArg (args[n], XmNposition, 2000); n++;
	XtSetArg (args[n], XmNwrap, TRUE); n++;
	year = XmCreateTextField (spin, "years", args, n);
	XtManageChild (year);

	/* The destination of the data transfer */
	n = 0;
	XtSetArg (args[n], XmNeditable, FALSE); n++;
	text = XmCreateTextField (rowcol, "drop-site", args, n);
	XtManageChild (text);

	/* Now program the UTM */
	XtAddCallback (day,    XmNconvertCallback,     convert_callback,     NULL);
	XtAddCallback (month,  XmNconvertCallback,     convert_callback,     NULL);
	XtAddCallback (year,   XmNconvertCallback,     convert_callback,     NULL);
	XtAddCallback (text,   XmNdestinationCallback, destination_callback, NULL);

	XtManageChild (spin);
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
