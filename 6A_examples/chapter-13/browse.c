/* browse.c -- specify a browse selection callback for a simple List. */

#include <Xm/List.h>

char *months[] = {"January", "February", "March", "April", "May", "June", "July", 
                  "August", "September", "October", "November", "December"};

main (int argc, char *argv[])
{
	Widget           toplevel, list_w;
	XtAppContext     app;
	int              i, n = XtNumber (months);
	XmStringTable    str_list;
	void             sel_callback(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	
	str_list = (XmStringTable) XtMalloc (n * sizeof (XmString *));

	for (i = 0; i < n; i++)
		str_list[i] = XmStringCreateLocalized (months[i]);
	
	list_w = XmCreateScrolledList (toplevel, "months", NULL, 0);
	XtVaSetValues (list_w,  XmNvisibleItemCount,  n,  XmNitemCount,  n,  XmNitems,  str_list,  NULL);
	XtManageChild (list_w);
	XtAddCallback (list_w, XmNdefaultActionCallback, sel_callback, NULL);
	XtAddCallback (list_w, XmNbrowseSelectionCallback, sel_callback, NULL);
	
	for (i = 0; i < n; i++)
		XmStringFree (str_list[i]);
	XtFree ((char *) str_list);
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
void sel_callback (Widget list_w, XtPointer client_data, XtPointer call_data)
{
	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	char *choice;
	
	if (cbs->reason == XmCR_BROWSE_SELECT)
		printf ("Browse selection -- ");
	else
		printf ("Default action -- ");
	
	choice = (char *) XmStringUnparse (cbs->item, XmFONTLIST_DEFAULT_TAG, 
					   XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL);
	printf ("selected item: %s (%d)\n", choice, cbs->item_position);
	XtFree (choice);
}
