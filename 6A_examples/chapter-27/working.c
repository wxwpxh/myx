/* working.c -- represent a complicated, time-consuming task by
** counting from 0 to 20000 and provide feedback to the user about
** how far we are in the process.  The user may terminate the process
** at any time by selecting the Stop button in the WorkingDialog.
** This demonstrates how a WorkingDialog can be used to allow the
** user to interrupt lengthy procedures.
*/

#include <Xm/MessageB.h>
#include <Xm/PushB.h>

#define MAXNUM 20000

void done(Widget, XtPointer, XtPointer);
/* Global variables */
static int          i = 0;
static XtWorkProcId work_id = (XtWorkProcId) 0 ;

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, button;
	XmString     label;
	Arg          args[2];
	int          n;
	void         pushed(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	
	label = XmStringCreateLocalized ("Press To Start A Long Task");
	n = 0;
	XtSetArg (args[n], XmNlabelString, label); n++;
	button = XmCreatePushButton (toplevel, "button", args, n);        
	XtAddCallback (button, XmNactivateCallback, pushed, (XtPointer) app);
	XmStringFree (label);
	XtManageChild (button);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* pushed() -- the callback routine for the main app's pushbutton. */
void pushed (Widget w, XtPointer client_data, XtPointer call_data)
{
	XtAppContext app = (XtAppContext) client_data;
	Widget       dialog;
	XmString     stop_txt;
	Arg          args[5];
	int          n;
	Boolean      count(XtPointer);
	
	/* Create the dialog -- the "cancel" button says "Stop" */
	n = 0;
	stop_txt = XmStringCreateLocalized ("Stop");
	XtSetArg(args[n], XmNcancelLabelString, stop_txt); n++;
	dialog = XmCreateWorkingDialog (w, "working", args, n);
	XmStringFree (stop_txt);
	
	work_id = XtAppAddWorkProc (app, count, dialog);
	XtUnmanageChild (XtNameToWidget (dialog, "OK"));
	XtUnmanageChild (XtNameToWidget (dialog, "Help"));
	
	/* Use cancel button to stop counting. True = remove work proc */
	XtAddCallback (dialog, XmNcancelCallback, done, (XtPointer) True);
	XtManageChild (dialog);
}

/* count() -- work procedure that counts to MAXNUM.  When we get there,
** change the "Stop" button to say "Done".
*/

Boolean count (XtPointer client_data)
{
	Widget   dialog = (Widget) client_data;
	char     buf[64];
	XmString str, button;
	Boolean  finished = False;
	
	/* If we printed every number, the flicker is too fast to read.
	** Therefore, just print every 1000 ticks for smoother feedback.
	*/

	if (++i % 1000 != 0)
		return finished;

	/* display where we are in the counter. */
	sprintf (buf, "Counter: %d", i);
	str = XmStringCreateLocalized (buf);
	XtVaSetValues (dialog, XmNmessageString, str, NULL);
	XmStringFree (str);
	
	if (i == MAXNUM) {
		i = 0;
		finished = True;
		button = XmStringCreateLocalized ("Done");
		XtVaSetValues (dialog, XmNcancelLabelString, button, NULL);
		XmStringFree (button);
		XtRemoveCallback (dialog, XmNcancelCallback, done, (XtPointer) True);
		XtAddCallback (dialog, XmNcancelCallback, done, (XtPointer) False);
		XMapRaised (XtDisplay (dialog), XtWindow (XtParent (dialog)));
	}
	
	/* Return either True, meaning we're done and remove the work proc,
	** or False, meaning continue working by calling this function.
	*/

	return finished;
}

/* done () -- user pressed "Stop" or "Done" in WorkingDialog. */
void done (Widget dialog, XtPointer client_data, XtPointer call_data)
{
	Boolean remove_work_proc = (Boolean) client_data;
	
	if (remove_work_proc) {
		i = 0;
		XtRemoveWorkProc (work_id);
	}
	
	XtDestroyWidget (XtParent (dialog));
}
