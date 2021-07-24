/* radio.c -- demonstrate a simple radio box. Create a
** box with 3 toggles: "one", "two" and "three".  The callback
** routine prints the most recently selected choice.  Maintain
** a global variable that stores the most recently selected.
*/

#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>

int toggle_item_set;

void toggled (Widget widget, XtPointer client_data, XtPointer call_data)
{
	int                           which = (int) client_data;
	XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;
	
	printf ("%s: %s\n", XtName (widget),
		state->set == XmSET ? "on" : state->set == XmOFF ? "off" : "indeterminate");
		
	if (state->set == XmSET)
		toggle_item_set = which;
	else
		toggle_item_set = 0;
}

main (int argc, char *argv[])
{
	Widget       toplevel, radio_box, one, two, three;
	XtAppContext app; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);
					 
	radio_box = XmCreateRadioBox (toplevel, "radio_box", NULL, 0);
	
	one = XmCreateToggleButtonGadget (radio_box, "One", NULL, 0);
	XtAddCallback (one, XmNvalueChangedCallback, toggled, (XtPointer) 1);
	XtManageChild (one);
	
	two = XmCreateToggleButtonGadget (radio_box, "Two", NULL, 0);
	XtAddCallback (two, XmNvalueChangedCallback, toggled, (XtPointer) 2);
	XtManageChild (two);
	
	three = XmCreateToggleButtonGadget (radio_box, "Three",  NULL, 0);
	XtAddCallback (three, XmNvalueChangedCallback, toggled, (XtPointer) 3);
	XtManageChild (three);
	
	XtManageChild (radio_box);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
