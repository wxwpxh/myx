/* entry_cb.c -- demonstrate how the XmNentryCallback resource works
** in RowColumn widgets. When a callback function is set for this
** resource, all the callbacks for the RowColumn's children are reset
** to point to this function.  Their original functions are no longer
** called had they been set in favor of the entry-callback function.
*/
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>

char *strings[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"};

void called (Widget widget, XtPointer client_data, XtPointer call_data)
{
	XmRowColumnCallbackStruct *cbs = (XmRowColumnCallbackStruct *) call_data;
	
	Widget pb = cbs->widget;
	
	printf ("%s: %d\n", XtName (pb), cbs->data);
}

static void never_called (Widget widget, XtPointer client_data, XtPointer call_data)
{
	puts ("This function is never called");
}

main (int argc, char *argv[])
{
	Widget       toplevel, parent, w;
	XtAppContext app;
	int          i;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);
					 
	parent = XmCreateRowColumn (toplevel, "rowcolumn", NULL, 0); 
	XtAddCallback (parent, XmNentryCallback, called, NULL); 
	
	/* simply loop through the strings creating a widget for each one */
	for (i = 0; i < XtNumber (strings); i++) {
		w = XmCreatePushButtonGadget (parent, strings[i], NULL, 0);
		
		/* Call XtAddCallback() to install client_data only! */
		XtAddCallback (w, XmNactivateCallback, never_called, (XtPointer) (i+1));
		XtManageChild (w);
	}
	
	XtManageChild (parent);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
