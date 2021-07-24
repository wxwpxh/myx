/* rowcol.c -- demonstrate a simple RowColumn widget. Create one
** with 3 pushbutton gadgets.  Once created, resize the thing in
** all sorts of contortions to get a feel for what RowColumns can
** do with its children.
*/

#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, button;
	XtAppContext app;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	rowcol = XmCreateRowColumn (toplevel, "rowcolumn", NULL, 0);   

	button = XmCreatePushButton (rowcol, "One", NULL, 0);
	XtManageChild (button);
	button = XmCreatePushButton (rowcol, "Two", NULL, 0);
	XtManageChild (button);
	
	button = XmCreatePushButton (rowcol, "Three", NULL, 0);
	XtManageChild (button);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
