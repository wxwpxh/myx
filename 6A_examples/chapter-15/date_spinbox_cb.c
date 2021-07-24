/* date_spinbox_cb.c -- demonstrate the spin box widget callbacks */

#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/SpinB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>

Widget day_t, month_t, year_t;

char *months[] = {
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
};

int month_days[2][12] = 
{
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

/* check_days: the XmNmodifyVerifyCallback for the SpinBox */
void check_days (Widget w, XtPointer client_data, XtPointer call_data)
{
	XmSpinBoxCallbackStruct *sb = (XmSpinBoxCallbackStruct *) call_data;
	int year ;
	int month ;
	int day ;
	int leap ;
	
	if (sb->widget == day_t) {
		/* Make sure that the new day never exceeds the maximum 
		** for the current month 
		*/
		XtVaGetValues (year_t, XmNposition, &year, 0) ;
	
		leap = (((year % 4) == 0) ? ((year % 400) == 0 ? 1 : 0) : 0);
	
		XtVaGetValues (month_t, XmNposition, &month, 0) ;
	
		if (sb->position > month_days[leap][month]) {
			if (sb->crossed_boundary)
				/* Going backwards */
				sb->position = month_days[leap][month];
			else
				/* Going forwards */
				sb->position = 1;
		}
	}
	else {
		/* The month or year has changed.
		** Recheck the day field to ensure it does not exceed the 
		** maximum for the new month or year.
		*/
		if (sb->widget == month_t) {
			month = sb->position ;
	
			XtVaGetValues (year_t, XmNposition, &year, 0);
		}
		else {
			year = sb->position ;
	
			XtVaGetValues (month_t, XmNposition, &month, 0);
		}
	
		leap = (((year % 4) == 0) ? ((year % 400) == 0 ? 1 : 0) : 0);
	
		XtVaGetValues (day_t, XmNposition, &day, 0);
	
		if (day > month_days[leap][month]) {
			XtVaSetValues (day_t, XmNposition, month_days[leap][month], 0);
		}
	}
}

/* print_date: the XmNvalueChangedCallback for the SpinBox */
void print_date (Widget w, XtPointer client_data, XtPointer call_data)
{
	XmSpinBoxCallbackStruct *sb = (XmSpinBoxCallbackStruct *) call_data;
	int day;
	int month;
	int year;
	
	if (sb->reason == XmCR_OK) {
		XtVaGetValues (day_t,   XmNposition, &day,   0);
		XtVaGetValues (month_t, XmNposition, &month, 0);
		XtVaGetValues (year_t,  XmNposition, &year,  0);
	
		printf ("New date: %d/%s/%d\n", day, months[month], year);
	}
}
	
main(int argc, char *argv[])
{
	Widget        toplevel, spin;
	XtAppContext  app;
	XmStringTable ampm;
	Arg           args[8];
	int           i, n;
	XmStringTable str_list;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	
	/* Create the SpinBox */
	spin = XmCreateSpinBox (toplevel, "spin", NULL, 0);
	
	/* Create the Day field */
	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmNUMERIC) ; n++;
	XtSetArg (args[n], XmNcolumns, 2) ; n++;
	XtSetArg (args[n], XmNeditable, FALSE) ; n++;
	XtSetArg (args[n], XmNminimumValue, 1) ; n++;
	XtSetArg (args[n], XmNmaximumValue, 31) ; n++; 
	XtSetArg (args[n], XmNposition, 1) ; n++;
	XtSetArg (args[n], XmNwrap, TRUE) ; n++;
	day_t = XmCreateTextField (spin, "dayText", args, n);
	XtManageChild (day_t);
	
	/* Create the Month field */
	
	n = XtNumber (months);
	str_list = (XmStringTable) XtMalloc ((unsigned) n * sizeof (XmString *));
	
	for (i = 0; i < n; i++)
		str_list[i] = XmStringCreateLocalized (months[i]);
	
	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmSTRING); n++;
	XtSetArg (args[n], XmNeditable, FALSE); n++;
	XtSetArg (args[n], XmNcolumns, 10); n++;
	XtSetArg (args[n], XmNwrap, TRUE); n++;
	XtSetArg (args[n], XmNvalues, str_list); n++;
	XtSetArg (args[n], XmNnumValues, XtNumber (months)); n++;
	month_t = XmCreateTextField (spin, "monthText", args, n);
	XtManageChild (month_t);
	
	for (i = 0; i < XtNumber (months); i++)
		XmStringFree (str_list[i]);
	XtFree( (XtPointer) str_list);
	
	/* Create the Year field */
	
	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmNUMERIC); n++;
	XtSetArg (args[n], XmNcolumns, 4); n++;
	XtSetArg (args[n], XmNeditable, FALSE); n++;
	XtSetArg (args[n], XmNminimumValue, 1990) ; n++;
	XtSetArg (args[n], XmNmaximumValue, 2010) ; n++; 
	XtSetArg (args[n], XmNposition, 2000) ; n++;
	XtSetArg (args[n], XmNwrap, TRUE); n++;
	year_t = XmCreateTextField (spin, "yearText", args, n);
	XtManageChild (year_t);
	
	XtManageChild (spin);
	XtAddCallback (spin, XmNmodifyVerifyCallback, check_days, NULL);
	XtAddCallback (spin, XmNvalueChangedCallback, print_date, NULL);
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
