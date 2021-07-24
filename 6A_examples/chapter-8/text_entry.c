/* text_entry.c -- This demo shows how the RowColumn widget can be 
** configured to build a text entry form.  It displays a table of 
** right-justified Labels and Text widgets that extend to the right 
** edge of the Form. 
*/

#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>

char *text_labels[] = {"Name:", "Phone:", "Address:", "City:", "State:", "Zip Code:"};

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, child;
	XtAppContext app;
	char         buf[8];
	int          i;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	rowcol = XmCreateRowColumn (toplevel, "rowcolumn",  NULL, 0);
	XtVaSetValues (rowcol, XmNpacking,  XmPACK_COLUMN,  
			       XmNnumColumns, XtNumber (text_labels),
			       XmNorientation, XmHORIZONTAL, 
			       XmNisAligned, True,
			       XmNentryAlignment, XmALIGNMENT_END,
			       NULL);

	/* simply loop through the strings creating a widget for each one */
	for (i = 0; i < XtNumber (text_labels); i++) {
		child = XmCreateLabelGadget (rowcol, text_labels[i],NULL, 0); 
		XtManageChild (child);
		
		sprintf (buf, "text_%d", i);
		child = XmCreateText (rowcol, buf, NULL, 0);
		XtManageChild (child);
	}
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
