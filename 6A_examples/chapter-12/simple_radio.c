/* simple_radio.c -- demonstrate a simple radio box by using
** XmVaCreateSimpleRadioBox(). Create a box with 3 toggles:
** "one", "two" and "three".  The callback routine prints
** the most recently selected choice.
*/

#include <Xm/RowColumn.h>

void toggled (Widget widget, XtPointer client_data, XtPointer call_data)
{
	int                           which = (int) client_data;
	XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;
	
	printf ("%s: %s\n", XtName (widget),
		state->set == XmSET? "on" : state->set == XmOFF ? "off" : "indeterminate");
}

main (int argc, char *argv[])
{
	Widget       toplevel, radio_box;
	XtAppContext app;
	XmString     one, two, three; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	one  = XmStringCreateLocalized ("One");
	two  = XmStringCreateLocalized ("Two");
	three  = XmStringCreateLocalized ("Three");
	
	radio_box = XmVaCreateSimpleRadioBox (toplevel, 
					      "radio_box",
					      0,  /* the inital choice */
					      toggled, /* the callback routine */
					      XmVaRADIOBUTTON, one,  NULL, NULL, NULL,
					      XmVaRADIOBUTTON, two,  NULL, NULL, NULL,
					      XmVaRADIOBUTTON, three, NULL, NULL, NULL,
					      NULL);

	XmStringFree (one);
	XmStringFree (two);
	XmStringFree (three);
	
	XtManageChild (radio_box);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
