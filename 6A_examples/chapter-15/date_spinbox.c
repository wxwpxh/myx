/* date_spinbox.c -- demonstrate the spin box widget */

#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/SpinB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <sys/types.h>
#include <sys/time.h>
	
Widget hours_t, mins_t, ampm_t;

main(int argc, char *argv[])
{
	Widget        toplevel, spin, child;
	XtAppContext  app;
	XmStringTable ampm;
	Arg           args[8];
	int           n;

	/* Initialize the SpinBox to the current time */
	long          tick  = time((long *) 0);
	struct tm    *tm    = localtime(&tick);
	
	/* 12 hour clock */
	int           hours = ((tm->tm_hour > 12) ? (tm->tm_hour - 12) : tm->tm_hour) ;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);

	/* Create the SpinBox */
	spin = XmCreateSpinBox (toplevel, "spin", NULL, 0);
	
	/* Create the Hours field */
	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmNUMERIC) ; n++;
	XtSetArg (args[n], XmNcolumns, 2) ; n++;
	XtSetArg (args[n], XmNeditable, FALSE) ; n++;
	XtSetArg (args[n], XmNminimumValue, 1) ; n++;
	XtSetArg (args[n], XmNmaximumValue, 12) ; n++; /* 12 hour clock */
	XtSetArg (args[n], XmNposition, hours) ; n++; /* The current time */
	XtSetArg (args[n], XmNwrap, TRUE) ; n++;
	
	hours_t = XmCreateTextField (spin, "hourText", args, n);
	XtManageChild (hours_t);

	/* Hours/Minutes separator */
	child = XmCreateLabel (spin, ":", NULL, 0);
	XtManageChild (child);
	
	/* Create the Minutes field */
	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType,XmNUMERIC); n++;
	XtSetArg (args[n], XmNcolumns, 2); n++;
	XtSetArg (args[n], XmNeditable, FALSE); n++;
	XtSetArg (args[n], XmNminimumValue, 0); n++;
	XtSetArg (args[n], XmNmaximumValue, 59); n++;
	XtSetArg (args[n], XmNwrap, TRUE); n++;
	XtSetArg (args[n], XmNposition, tm->tm_min) ; n++;
	mins_t = XmCreateTextField (spin, "minuteText", args, n);
	XtManageChild (mins_t);
	
	/* Create the am/pm indicator field */
	ampm = (XmStringTable) XtMalloc(2 * sizeof (XmString *));
	ampm[0] = XmStringCreateLocalized ("am");
	ampm[1] = XmStringCreateLocalized ("pm");
	
	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmSTRING); n++;
	XtSetArg (args[n], XmNcolumns, 2); n++;
	XtSetArg (args[n], XmNeditable, FALSE); n++;
	XtSetArg (args[n], XmNnumValues, 2); n++;
	XtSetArg (args[n], XmNvalues, ampm); n++;
	XtSetArg (args[n], XmNwrap, TRUE); n++;
	/* Current time of day */
	XtSetArg (args[n], XmNposition, (tm->tm_hour > 12) ? 1 : 0); n++;
	ampm_t = XmCreateTextField (spin, "ampmText", args, n);
	XtManageChild (ampm_t);
	XmStringFree (ampm[0]);
	XmStringFree (ampm[1]);
	XtFree ((char *) ampm);
	
	XtManageChild (spin);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
