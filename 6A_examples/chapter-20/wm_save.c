/* wm_save.c -- demonstrate how to save the state of an application
** from a WM_SAVE_YOURSELF session manager protocol.  This is not a
** real program -- just a template.
*/

#include <Xm/Xm.h>
#include <Xm/Protocols.h>
#include <stdio.h>

/* save the original argc and argv for possible WM_SAVE_YOURSELF messages */
int    save_argc = 0;
char **save_argv = (char **) 0;

main (int argc, char *argv[])
{
	Widget       toplevel;
	XtAppContext app;
	Atom         WM_SAVE_YOURSELF;
	void         save_state(Widget, XtPointer, XtPointer);
	char        *restart_file;
	int          i;
	
	/* save argc and argv values */
	save_argv = (char **) XtMalloc (argc * sizeof(char *));
	
	for (i = save_argc = 0; i < argc; i++) {
		/* we don't need to save old -restart options */
		if (!strcmp (argv[i], "-restart"))
			i++; /* next arg is filename */
		else
			save_argv[save_argc++] = strcpy (XtMalloc (strlen(argv[i]) + 1), 
							 argv[i]);
	}
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	/* initialize toolkit: argv has its Xt-specific args stripped */
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass,  
					XmNwidth, 100, XmNheight, 100, NULL);
					
	/* get the WM_SAVE_YOURSELF protocol atom and register it with the
	** toplevel window's WM_PROTOCOLS property.  Also add a callback.
	*/
	
	WM_SAVE_YOURSELF = XInternAtom (XtDisplay (toplevel), "WM_SAVE_YOURSELF", False);
	XmAddWMProtocols (toplevel, &WM_SAVE_YOURSELF, 1);
	XmAddWMProtocolCallback (toplevel, WM_SAVE_YOURSELF, save_state, 
				(XtPointer) toplevel) ;

	/* create widgets... */
	
	/* now check to see if we are restarting from a previously run state */
	for (i = 0; i < argc; i++) {
		if (!strcmp (argv[i], "-restart")) {
			/* restarting from a previously saved state */
			restart_file = argv[++i];
		}
		
		/* possibly process other args here, too */
	}
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* called if WM_SAVE_YOURSELF client message was sent...
*/

void save_state (Widget widget, XtPointer client_data, XtPointer call_data)
{
	Widget toplevel = (Widget) client_data;
	/* hypothetical function */
	extern char *SaveStateAndReturnFileName(); 
	char  *filename = SaveStateAndReturnFileName ();
	
	puts("save_state()");
	
	save_argv = (char **) XtRealloc ((char *) save_argv, (save_argc+2) * sizeof (char *));
	save_argv[save_argc++] = "-restart";
	save_argv[save_argc++] = filename;
	
	/* notice the order of XSetCommand() args! */
	XSetCommand (XtDisplay (toplevel), XtWindow (toplevel), save_argv, save_argc);
}
