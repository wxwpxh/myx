/* spreadsheet.c -- This demo shows the most basic use of the RowColumn
** It displays a table of widgets in a row-column format similar to a
** spreadsheet. This is accomplished by setting the number ROWS and
** COLS and setting the appropriate resources correctly.
*/

#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

#define ROWS  8
#define COLS 10

main (int argc, char *argv[])
{
	Widget       toplevel, parent, child;
	XtAppContext app;
	char         buf[16];
	int          i, j; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	parent = XmCreateRowColumn (toplevel, "rowcolumn", NULL, 0);
	XtVaSetValues (parent, XmNpacking,  XmPACK_COLUMN,  
			       XmNnumColumns, COLS,
			       XmNorientation, XmVERTICAL,
			       NULL);

	/* simply loop through the strings creating a widget for each one */
	for (i = 0; i < COLS; i++)
		for (j = 0; j < ROWS; j++) {
			sprintf (buf, "%d-%d", i+1, j+1);
			
			if (i == 0 || j == 0)
				child = XmCreateLabelGadget (parent, buf, NULL, 0);
			else
				child = XmCreatePushButton (parent, "", NULL, 0);
			XtManageChild (child);
		}
		
	XtManageChild (parent);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
