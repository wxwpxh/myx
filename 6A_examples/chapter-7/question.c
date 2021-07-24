/* question.c - create a QuestionDialog with four action buttons */

#include <Xm/MessageB.h>
#include <Xm/PushB.h>

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, pb;
	void         pushed(Widget, XtPointer, XtPointer);
	 
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					  sessionShellWidgetClass, NULL);   

	pb = XmCreatePushButton (toplevel, "Button", NULL, 0);
	XtAddCallback (pb, XmNactivateCallback, pushed, NULL);
	XtManageChild (pb);
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

void pushed (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget dialog, no_button;
	void   dlg_callback(Widget, XtPointer, XtPointer);
	Arg    args[5];
	int    n = 0;
	
	XmString m = XmStringCreateLocalized ("Do you want to update your changes?");
	XmString yes = XmStringCreateLocalized ("Yes");
	XmString no = XmStringCreateLocalized ("No");
	
	XtSetArg (args[n], XmNautoUnmanage, False); n++; 
	XtSetArg (args[n], XmNmessageString, m); n++;
	XtSetArg (args[n], XmNokLabelString, yes); n++;
	dialog = XmCreateQuestionDialog (w, "notice", args, n);
	XtAddCallback (dialog, XmNokCallback, dlg_callback, NULL);
	XtAddCallback (dialog, XmNcancelCallback, dlg_callback, NULL);
	XtAddCallback (dialog, XmNhelpCallback, dlg_callback, NULL);
	XmStringFree (m);
	XmStringFree (yes);
	
	XtSetArg(args[0], XmNlabelString, no);
	no_button = XmCreatePushButton (dialog, "no", args, 1);
	XtAddCallback (no_button, XmNactivateCallback, dlg_callback, NULL);
	
	XtManageChild (no_button);
	XtManageChild (dialog);
}

void dlg_callback (Widget w, XtPointer client_data, XtPointer call_data)
{
	XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;
	
	switch (cbs->reason) {
		case XmCR_OK       : /* FALLTHROUGH */ 
		case XmCR_CANCEL   : XtUnmanageChild (w); break;
		case XmCR_ACTIVATE : XtUnmanageChild (XtParent (w)); break;
		case XmCR_HELP     : puts ("Help selected"); break;
	}
}
