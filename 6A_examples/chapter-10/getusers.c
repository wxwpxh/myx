/* getusers.c -- demonstrate a simple ScrolledWindow by showing
** how it can manage a RowColumn that contains a vertical stack of
** Form widgets, each of which contains a Toggle, two Labels and
** a Text widget. The program fills the values of the widgets
** using various pieces of information from the password file.
** Note: there are no callback routines associated with any of the
** widgets created here -- this is for demonstration purposes only.
*/
#include <Xm/PushBG.h>
#include <Xm/LabelG.h>
#include <Xm/ToggleB.h>
#include <Xm/ScrolledW.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Text.h>
#include <pwd.h>

typedef struct {
	String login;
	int    uid;
	String name;
	String homedir;
} UserInfo;

/* use getpwent() to read data in the password file to store
** information about all the users on the system. The list is
** a dynamically grown array, the last of which has a NULL login.
*/

UserInfo *getusers(void)
{
	/* extern struct *passwd getpwent(); */
	extern char *strcpy();
	struct passwd *pw;
	UserInfo      *users = NULL;
	int            n;
	
	setpwent();
	
	/* getpwent() returns NULL when there are no more users */
	for (n = 0; pw = getpwent(); n++) {
		/* reallocate the pointer to contain one more entry. You may choose
		** to optimize by adding 10 entries at a time, or perhaps more?
		*/
		
		users = (UserInfo *) XtRealloc ((char *) users, (n+1) * sizeof (UserInfo));
		users[n].login = strcpy (XtMalloc   (strlen (pw->pw_name)+1), pw->pw_name);
		users[n].name = strcpy (XtMalloc (strlen (pw->pw_gecos)+1), pw->pw_gecos);
		users[n].homedir = strcpy (XtMalloc (strlen (pw->pw_dir)+1), pw->pw_dir);
		users[n].uid = pw->pw_uid;
	}

	/* allocate one more item and set its login string to NULL */
	users = (UserInfo *) XtRealloc ((char *) users, (n+1) * sizeof (UserInfo));
	users[n].login = NULL;
	endpwent();
	
	return users; /* return new array */
}

main (int argc, char *argv[])
{
	Widget       toplevel, sw, main_rc, form, toggle, child; 
	XtAppContext app;
	UserInfo    *users;
	Arg          args[10];
	int          n;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	/* Create a 500x300 scrolled window. This value is arbitrary,
	** but happens to look good initially. It is resizable by the user.
	*/
	
	n = 0;
	XtSetArg (args[n], XmNwidth, 500); n++;
	XtSetArg (args[n], XmNheight, 300); n++;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
	sw = XmCreateScrolledWindow (toplevel, "scrolled_w",   args, n);
	
	/* RowColumn is the work window for the widget */
	main_rc = XmCreateRowColumn (sw, "main_rc", NULL, 0);
	
	/* load the users from the passwd file */
	if (!(users = getusers())) {
		perror ("Can't read user data info");
		exit (1);
	}
	
	/* for each login entry found in the password file, create a
	** form containing a toggle button, two labels and a text widget.
	*/
	
	while (users->login) {
		/* NULL login terminates list */ 
		char uid[8];
		
		form = XmCreateForm (main_rc, "", NULL, 0);
		
		n = 0;
		XtSetArg (args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
		XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNrightAttachment, XmATTACH_POSITION); n++;
		XtSetArg (args[n], XmNrightPosition, 15); n++;
		child = XmCreateToggleButton (form, users->login, args, n);
		XtManageChild (child);

		sprintf (uid, "%d", users->uid);
		n = 0;
		XtSetArg (args[n], XmNalignment, XmALIGNMENT_END); n++;
		XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
		XtSetArg (args[n], XmNleftPosition, 15); n++;
		XtSetArg (args[n], XmNrightAttachment, XmATTACH_POSITION); n++;
		XtSetArg (args[n], XmNrightPosition, 20); n++;
		child = XmCreateLabelGadget (form, uid, args, n);
		XtManageChild (child);

		n = 0;
		XtSetArg (args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
		XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
		XtSetArg (args[n], XmNleftPosition, 20); n++;
		XtSetArg (args[n], XmNrightAttachment, XmATTACH_POSITION); n++;
		XtSetArg (args[n], XmNrightPosition, 50); n++;
		child = XmCreateLabelGadget (form, users->name, args, n);
		XtManageChild (child);

		/* Although the home directory is readonly, it may be longer
		** than expected, so don't use a Label widget. Use a Text widget
		** so that left-right scrolling can take place.
		*/
		n = 0;
		XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
		XtSetArg (args[n], XmNleftPosition, 50); n++;
		XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
		XtSetArg (args[n], XmNvalue, users->homedir); n++;
		XtSetArg (args[n], XmNeditable, False); n++;
		XtSetArg (args[n], XmNcursorPositionVisible, False); n++;
		child = XmCreateText (form, users->homedir, args, n);
		XtManageChild (child);

		XtManageChild (form);
		users++;
	}

	XtManageChild (main_rc);
	XtManageChild (sw);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
