/* show_files.c -- introduce FileSelectionDialog; print the file
** selected by the user.
*/
#include <Xm/FileSB.h>

main (int argc, char *argv[])
{
	Widget       toplevel, text_w, dialog;    
	XtAppContext app;
	extern  void exit(int);
	void         echo_file(Widget, XtPointer, XtPointer);
	Arg          args[2];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);  

	/* Create a simple Motif 2.1 FileSelectionDialog */
	XtSetArg (args[0], XmNpathMode, XmPATH_MODE_RELATIVE);
	dialog = XmCreateFileSelectionDialog (toplevel, "filesb", args, 1);
	XtAddCallback (dialog, XmNcancelCallback, (void (*)()) exit, NULL);
	XtAddCallback (dialog, XmNokCallback, echo_file, NULL);
	XtManageChild (dialog);
	XtAppMainLoop (app);
}

/* callback routine when the user selects OK in the FileSelection
** Dialog. Just print the file name selected.
**/
void echo_file (Widget widget, /* file selection box */
                XtPointer client_data,
		XtPointer call_data)
{
	char *filename;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

	filename = (char *) XmStringUnparse (cbs->value, XmFONTLIST_DEFAULT_TAG, 
					      XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL);

	if (!filename)
		return;
		
	/* must have been an internal error */
	
	if (!*filename) {
		/* nothing typed? */
		puts ("No file selected.");

		XtFree (filename);
		
		/* even "" is an allocated byte */
		return;
	}

	printf ("Filename given: \"%s\"\n", filename);
	XtFree (filename);
}
