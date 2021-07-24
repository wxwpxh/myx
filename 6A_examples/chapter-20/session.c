/* session.c - outlines the interactions with ther session manager
*/

#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/SSpinB.h>
#include <Xm/MessageB.h>

Widget toplevel, spin;

char *restart_command[6] = {
	NULL,
	"-xtsessionID",
	NULL,
	"-value",
	NULL,
	NULL
};

/* The "OK" button is pressed in the popup interaction dialog */
/* This does not perform save-yourself actions -              */
/* it informs the session manager that we need to do so       */
static void msg_ok_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
	XtCheckpointToken token = (XtCheckpointToken) client_data;

	/* Asks the session manager to call the save_callback */
	token->save_success = True;
	/* Return the token */
	XtSessionReturnToken (token);
}

/* The "Cancel" button is pressed in the popup interaction dialog */
static void msg_cancel_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
	XtCheckpointToken token = (XtCheckpointToken) client_data;

	/* Tells the session manager not to call the save_callback */
	token->request_cancel = True;
	token->save_success = False;
	/* Return the token */
	XtSessionReturnToken (token);
}

/* Interacts with the user during session shell operations */
static void interact_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
	static Widget            message = (Widget) 0;
	       XtCheckpointToken token   = (XtCheckpointToken) call_data;
	       XmString          xms;
	       Arg               args[8];
	       int               n;

	if (token->cancel_shutdown || token->interact_style == None) {
		token->save_success = False;
		return;
	}

	if (message == (Widget) 0) {
		n = 0;
		xms = XmStringCreateLocalized ("Save Changes Before Quitting?");
		XtSetArg (args[n], XmNmessageString, xms) ; n++;

		message = XmCreateQuestionDialog (toplevel, "message", args, n);

		XtUnmanageChild (XtNameToWidget (message, "Help")) ;
		
		XtAddCallback (XtNameToWidget (message, "OK"),     XmNactivateCallback, msg_ok_callback,     call_data);
		XtAddCallback (XtNameToWidget (message, "Cancel"), XmNactivateCallback, msg_cancel_callback, call_data);
	}

	XtManageChild (message);
}

/* Performs the session manager save-yourself actions */
/* That is, it sets up the XtNrestartCommand array as appropriate */
static void save_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
	XtCheckpointToken token = (XtCheckpointToken) call_data;
	int spin_value;
	char buf[20];

	XtVaGetValues (spin, XmNposition, &spin_value, NULL);
	(void) sprintf(buf, "%d", spin_value);
	restart_command[4] = buf;
	XtVaSetValues (toplevel, XtNrestartCommand, restart_command, NULL);

	if (token->interact_style != SmInteractStyleNone) {
		if (token->interact_style == SmInteractStyleAny)
			token->interact_dialog_type = SmDialogNormal;
		else
			token->interact_dialog_type = SmDialogError;

		XtAddCallback (toplevel, XtNinteractCallback, interact_callback, NULL);
	}
}

/* Signals to the session manager that save completed successfully */
/* In this example, there is nothing to do */
static void save_complete_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
	XtCheckpointToken token = (XtCheckpointToken) call_data;
}

/* Kills this application in response to session manager request */
static void die_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
	XtDestroyWidget (toplevel);
	exit (0);
}

main (int argc, char *argv[])
{
	XtAppContext app;
	Arg          args[16];
	int          i, n;
	String       smcid;
	int          spin_value = 0;

	/* Parse the command-line arguments for -value nnn. */
	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "-value") == 0) && (i < argc - 1)) {
			spin_value = atoi(argv[++i]);
		}
	}

	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, args, n);

	/* Set up the restart command */
	XtVaGetValues(toplevel, XtNsessionID, &smcid, NULL);

	restart_command[0] = argv[0];
	restart_command[2] = XtNewString(smcid);
	restart_command[4] = "0";
	XtVaSetValues(toplevel, XtNrestartCommand, restart_command, NULL);

	XtAddCallback( toplevel, XtNsaveCallback, save_callback, NULL );
	XtAddCallback( toplevel, XtNcancelCallback, save_complete_callback, NULL );
	XtAddCallback( toplevel, XtNsaveCompleteCallback, save_complete_callback, NULL );
	XtAddCallback( toplevel, XtNdieCallback, die_callback, NULL );

	n = 0;
	XtSetArg (args[n], XmNspinBoxChildType, XmNUMERIC) ; n++;
	XtSetArg (args[n], XmNeditable, FALSE) ; n++;
	XtSetArg (args[n], XmNposition, spin_value) ; n++;
	XtSetArg (args[n], XmNminimumValue, 0) ; n++;
	XtSetArg (args[n], XmNmaximumValue, 99) ; n++;
	XtSetArg (args[n], XmNwrap, TRUE) ; n++;
	
	spin = XmCreateSimpleSpinBox (toplevel, "spin", args, n);
	
	XtManageChild (spin);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

