/* combo_cb.c -- demonstrate the combobox widget selection callback */

#include <Xm/Xm.h>
#include <Xm/ComboBox.h>

/* the list of colors */
char *colors[] = {
	"red", "green", "blue", "orange", "purple", "pink", "white", "black", "yellow"
};

/* selection_callback: simply prints out the current ComboBox selection */
void selection_callback (Widget w, XtPointer client_data, XtPointer call_data)
{
	XmComboBoxCallbackStruct *cb = (XmComboBoxCallbackStruct *) call_data;

	char *choice = (char *) XmStringUnparse (cb->item_or_text, XmFONTLIST_DEFAULT_TAG,
                                                 XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL);

	if (cb->event == NULL) {
		printf ("Browsing: potential choice is %s\n", choice);
	}
	else {
		printf ("New current choice is %s\n", choice);
	}

	XtFree(choice);
}

main (int argc, char *argv[])
{
	Widget        toplevel, combo;
	XtAppContext  app;
	Arg           args[4];
	int           count = XtNumber (colors);
	int           i, n;
	XmStringTable str_list;

	/* initialize the toolkit */
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	/* create the List items */
	str_list = (XmStringTable) XtMalloc ((unsigned) count * sizeof (XmString *));

	for (i = 0; i < count; i++)
		str_list[i] = XmStringCreateLocalized (colors[i]);

	/* create the combobox */
	n = 0;
	XtSetArg (args[n], XmNitems, str_list); n++;
	XtSetArg (args[n], XmNitemCount, count); n++;
	combo = XmCreateDropDownComboBox (toplevel, "combo", args, n);
	
	for (i = 0; i < n; i++)
		XmStringFree (str_list[i]);
	XtFree ((XtPointer) str_list);

	XtAddCallback (combo, XmNselectionCallback, selection_callback, NULL);
	
	XtManageChild (combo);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
