/* alpha_list.c -- insert items into a list in alphabetical order.
*/

#include <Xm/List.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>

main (int argc, char *argv[])
{
	Widget        toplevel, rowcol, list_w, text_w;
	XtAppContext  app;
	Arg           args[5];
	int           n = 0;
	void          add_item(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
					
	rowcol = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);
	XtSetArg (args[n], XmNvisibleItemCount, 5); n++;
	list_w = XmCreateScrolledList (rowcol, "scrolled_list", args, n);
	XtManageChild (list_w);
	
	n = 0;
	XtSetArg (args[n], XmNcolumns, 25); n++;
	text_w = XmCreateTextField (rowcol, "text", args, n);
	XtAddCallback (text_w, XmNactivateCallback, add_item, (XtPointer) list_w);
	XtManageChild (text_w);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* Add item to the list in alphabetical order.  Perform binary
** search to find the correct location for the new item position.
** This is the callback routine for the TextField widget.
*/
void add_item (Widget text_w, XtPointer client_data, XtPointer call_data)
{
	Widget   list_w = (Widget) client_data; 
	char    *text, *newtext = XmTextFieldGetString (text_w);
	XmString str, *strlist;
	int      u_bound, l_bound = 0;
	
	/* newtext is the text typed in the TextField widget */
	
	if (!newtext || !*newtext) {
		/* non-null strings must be entered */
		XtFree (newtext); /* XtFree() checks for NULL */
		return;
	}
	
	/* get the current entries (and number of entries) from the List */
	XtVaGetValues (list_w, XmNitemCount, &u_bound, XmNitems, &strlist, NULL);
	u_bound--;
	
	/* perform binary search */
	while (u_bound >= l_bound) {
		int i = l_bound + (u_bound - l_bound) / 2;
		
		/* convert the compound string into a regular C string */
		
		if (!(text = (char *) XmStringUnparse (strlist[i], XmFONTLIST_DEFAULT_TAG,
						       XmCHARSET_TEXT, XmCHARSET_TEXT, 
						       NULL, 0, XmOUTPUT_ALL)))
			break;
			
		if (strcmp (text, newtext) > 0)
			u_bound = i - 1; /* newtext comes before item */
		else
			l_bound = i + 1; /* newtext comes after item */
			
		XtFree (text);  /* XmStringUnparse() allocates memory */
	}

	str = XmStringCreateLocalized (newtext); 
	
	
	XtFree (newtext); 
	
	/* positions indexes start at 1, so increment accordingly */
	XmListAddItemUnselected (list_w, str, l_bound+1);
	XmStringFree (str);
	XmTextFieldSetString (text_w, "");
}
