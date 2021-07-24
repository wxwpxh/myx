/* toggle_box.c -- demonstrate a homebrew ToggleBox.  A static
** list of strings is used as the basis for a list of toggles.
** The callback routine toggled() is set for each toggle item.
** The client data for this routine is set to the enumerated
** value of the item with respect to the entire list.  This value
** is treated as a bit which is toggled in "toggles_set" -- a
** mask that contains a complete list of all the selected items.
** This list is printed when the PushButton is selected.
*/

#include <Xm/ToggleBG.h>
#include <Xm/PushBG.h>
#include <Xm/SeparatoG.h>
#include <Xm/RowColumn.h>

unsigned long toggles_set = (unsigned long) 0 ; /* has the bits of which toggles are set */

char *strings[] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten" };

/* A RowColumn is used to manage a ToggleBox (also a RowColumn) and
** a PushButton with a separator gadget in between.
*/

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, toggle_box, w;
	XtAppContext app;
	void         toggled(Widget, XtPointer, XtPointer) ;
	void         check_bits(Widget, XtPointer, XtPointer);
	int          i; 
	Arg          args[4];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos",  NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
					
	i = 0;
	XtSetArg (args[i], XmNpacking, XmPACK_TIGHT); i++;
	rowcol = XmCreateRowColumn (toplevel, "rowcolumn", args, i);
	
	i = 0;
	XtSetArg (args[i], XmNpacking, XmPACK_COLUMN); i++;
	XtSetArg (args[i], XmNnumColumns, 2); i++;
	toggle_box = XmCreateRowColumn (rowcol, "togglebox", args, i);
	
	/* simply loop thru the strings creating a widget for each one */
	for (i = 0; i < XtNumber (strings); i++) {
		w = XmCreateToggleButtonGadget (toggle_box, strings[i], NULL, 0);
		XtAddCallback (w, XmNvalueChangedCallback, toggled, (XtPointer) i);
		XtManageChild (w);
	}
	
	w = XmCreateSeparatorGadget (rowcol, "sep",NULL, 0);
	XtManageChild (w);
	
	w = XmCreatePushButtonGadget (rowcol, "Check Toggles",NULL, 0); 
	XtAddCallback (w, XmNactivateCallback, check_bits, NULL);
	XtManageChild (w);
	
	XtManageChild (rowcol);
	XtManageChild (toggle_box);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* callback for all ToggleButtons.*/
void toggled (Widget widget, XtPointer client_data, XtPointer call_data)
{
	int bit = (int) client_data;
	XmToggleButtonCallbackStruct *toggle_data = (XmToggleButtonCallbackStruct *) call_data;

	if (toggle_data->set == XmSET) /* if the toggle button is set, flip its bit */
		toggles_set |= (1 << bit);
	else /* if the toggle is "off", turn off the bit. */
		toggles_set &= ~(1 << bit);
}

void check_bits (Widget widget, XtPointer client_data, XtPointer call_data)
{
	int i;

	printf ("Toggles set:");

	for (i = 0; i < XtNumber (strings); i++)
		if (toggles_set & (1<<i))
			printf (" %s", strings[i]);

	putchar ('\n');
}
