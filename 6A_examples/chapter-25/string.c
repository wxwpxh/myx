#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>

String fallbacks[] = { "*MY_TAG.fontName:9x15", NULL };

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, push_b ;
	XtAppContext app;
	XmString     text;
	Display     *dpy;
	Arg          args[2];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demo", NULL, 0, &argc, argv, fallbacks, 
					sessionShellWidgetClass, NULL);
					
	rowcol = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);
	
	text = XmStringGenerate ((XtPointer) "Testing, testing...",
				 XmFONTLIST_DEFAULT_TAG,
				 XmCHARSET_TEXT,
				 "MY_TAG");

	XtSetArg (args[0], XmNlabelString, text);
	
	push_b = XmCreatePushButtonGadget (rowcol, "pb1", args, 1);
	XtManageChild (push_b);
	
	push_b = XmCreatePushButtonGadget (rowcol, "pb2", args, 1);
	XtManageChild (push_b);
	
	push_b = XmCreatePushButtonGadget (rowcol, "pb3", args, 1);
	XtManageChild (push_b);
	
	XmStringFree (text);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
