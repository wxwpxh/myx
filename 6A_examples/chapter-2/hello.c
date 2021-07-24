/* hello.c -- initialize the toolkit using an application context and a 
** toplevel shell widget, then create a pushbutton that says Hello using 
** the varargs interface. 
*/
#include <Xm/PushB.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	Widget       toplevel, button;
	XtAppContext app; 
	void         button_pushed(Widget, XtPointer, XtPointer);
	XmString     label;
	Arg          args[2]; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Hello", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	label = XmStringCreateLocalized ("Push here to say hello");
	XtSetArg(args[0], XmNlabelString, label);
	button = XmCreatePushButton (toplevel, "pushme", args, 1);
	XmStringFree (label);

	XtAddCallback (button, XmNactivateCallback, button_pushed, NULL);
	XtManageChild (button);

	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
	
	return 0;
}

void button_pushed (Widget widget, XtPointer client_data, XtPointer call_data)
{
	printf ("Hello Yourself!\n");
}
