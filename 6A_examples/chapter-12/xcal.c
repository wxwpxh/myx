/* xcal.c -- display a monthly calendar. The month displayed is a
** single Label widget whose text is generated from the output of
** the "cal" program found on any UNIX machine.  popen() is used
** to run the program and read its output.  Although this is an
** inefficient method for getting the output of a separate program,
** it suffices for demonstration purposes.  A List widget displays
** the months and the user can provide the year as argv[1].
*/

#include <stdio.h>
#include <X11/Xos.h>
#include <Xm/Xm.h>
#include <Xm/List.h>
#include <Xm/Frame.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/SeparatoG.h>

int           year = 0 ;
XmStringTable ArgvToXmStringTable(int, char **);
void          FreeXmStringTable(XmStringTable);
char         *months[] = { "January", "February", "March", "April", "May", "June", "July", 
		           "August", "September", "October", "November", "December" };

main (int argc, char *argv[])
{
	Widget        toplevel, frame, rowcol, label, w;
	XtAppContext  app;
	void          set_month(Widget, XtPointer, XtPointer);
	XmRenderTable render_table;
	XmRendition   rendition;
	Arg           args[10];
	int           n;
	XmStringTable strs;
	int           month_no;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	/* Create a render table based on the fonts we're using.  These are the
	** fonts that are going to be hardcoded in the Label and List widgets.
	*/
	
	n = 0;
	XtSetArg (args[n], XmNfontName, "-*-courier-bold-r-*--18-*"); n++;
	XtSetArg (args[n], XmNfontType, XmFONT_IS_FONT); n++;
	XtSetArg (args[n], XmNloadModel, XmLOAD_IMMEDIATE); n++;
	rendition = XmRenditionCreate (toplevel, "tag1", args, n);
	render_table  = XmRenderTableAddRenditions(NULL, &rendition, 1, XmMERGE_NEW);
	XmRenditionFree(rendition);
	
	n = 0;
	XtSetArg (args[n], XmNfontName, "-*-courier-medium-r-*--18-*"); n++;
	XtSetArg (args[n], XmNfontType, XmFONT_IS_FONT); n++;
	XtSetArg (args[n], XmNloadModel, XmLOAD_IMMEDIATE); n++;
	rendition = XmRenditionCreate (toplevel, "tag2", args, n);
	render_table  = XmRenderTableAddRenditions(render_table, &rendition, 1, XmMERGE_NEW);
	XmRenditionFree(rendition);
	
	if (argc > 1) {
		month_no = 1;year = atoi (argv[1]);
	} else {
		extern long time(long *);
		
		long t = time((long *) 0);
		struct tm *today = localtime (&t);
		year = 1900 + today->tm_year;
		month_no = today->tm_mon+1;
	}
	
	/* The RowColumn is the general layout manager for the application.
	** It contains two children: a Label gadget that displays the calendar
	** month, and a ScrolledList to allow the user to change the month.
	*/
	
	n = 0;
	XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
	rowcol = XmCreateRowColumn (toplevel, "rowcol", args, n);
	
	/* enclose the month in a Frame for decoration. */
	frame = XmCreateFrame (rowcol, "frame", NULL, 0);
	
	n = 0;
	XtSetArg (args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
	XtSetArg (args[n], XmNrenderTable, render_table); n++;
	label = XmCreateLabelGadget (frame, "month", args, n);
	XtManageChild (label);
	XtManageChild (frame);
	
	/* create a list of month names */
	strs = ArgvToXmStringTable (XtNumber (months), months);
	w = XmCreateScrolledList (rowcol, "list", NULL, 0);
	XtVaSetValues (w, XmNitems, strs, 
			  XmNitemCount, XtNumber(months), 
			  XmNrenderTable, render_table, 
			  NULL);
			  
	FreeXmStringTable (strs);
	XmRenderTableFree (render_table);
	XtAddCallback (w, XmNbrowseSelectionCallback, set_month, (XtPointer) label); 
	XtManageChild (w);
	XmListSelectPos (w, month_no, True); 
	
	/* initialize month */
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* callback function for the List widget -- change the month */
void set_month (Widget w, XtPointer client_data, XtPointer call_data)
{
	register FILE *pp;
	extern   FILE *popen(const char *, const char *);
	char     text[BUFSIZ];
	register char *p = text;
	XmString str;
	Widget   label = (Widget) client_data;
	XmListCallbackStruct *list_cbs = (XmListCallbackStruct *) call_data;

	/* Ask UNIX to execute the "cal" command and read its output */
	sprintf (text, "cal %d %d", list_cbs->item_position, year);
	
	if (!(pp = popen (text, "r"))) {
		perror (text);
		return;
	}
	*p = 0;
	
	while (fgets (p, sizeof (text) - strlen (text), pp))
		p += strlen (p);
	pclose (pp);
	
	/* display the month using the "tag1" rendition from the
	** Label gadget's XmNrenderTable
	*/
	
	str = XmStringGenerate((XtPointer) text, "tag1", XmCHARSET_TEXT, NULL);
	XtVaSetValues (label, XmNlabelString, str, NULL);
	XmStringFree (str);
}

/* Convert an array of string to an array of compound strings */
XmStringTable ArgvToXmStringTable(int argc, char **argv)
{
	XmStringTable new = (XmStringTable) XtMalloc ((argc+1) * sizeof (XmString));
	
	if (!new)
		return (XmStringTable) 0;
	new[argc] = (XmString) 0;
	
	while (--argc >= 0)
		new[argc] = XmStringGenerate ((XtPointer) argv[argc], "tag2", XmCHARSET_TEXT, NULL);
		
	return new;
}

/* Free the table created by ArgvToXmStringTable() */
void FreeXmStringTable (XmStringTable argv)
{
	register int i;

	if (!argv)
		return;

	for (i = 0; argv[i]; i++)
		XmStringFree (argv[i]);
		
	XtFree ((char *) argv);
}
