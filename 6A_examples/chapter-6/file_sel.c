/* file_sel.c -- file selection dialog displays a list of all the writable
** files in the directory described by the XmNmask of the dialog. 
** This program demonstrates how to use the XmNfileSearchProc for 
** file selection dialog widgets.
*/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/FileSB.h>
#include <Xm/DialogS.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <X11/Xos.h>
#include <sys/stat.h> 

void do_search(Widget, XtPointer, XtPointer);
void new_file_cb(Widget, XtPointer, XtPointer);

/* routine to determine if a file is accessible, a directory,
** or writable. Return -1 on all errors or if the file is not 
** writable. Return 0 if it's a directory or 1 if it's a plain
** writable file. 
*/
int is_writable (char *file)
{
	struct stat s_buf;
	
	/* if file can't be accessed (via stat()) return. */
	
	if (stat (file, &s_buf) == -1)
		return -1;
	else if ((s_buf.st_mode & S_IFMT) == S_IFDIR)
		return 0; /* a directory */
	else if (!(s_buf.st_mode & S_IFREG) || access (file, W_OK) == -1)
		/* not a normal file or it is not writable */
		return -1;

	/* legitimate file */
	
	return 1;
}

/* main() -- create a FileSelectionDialog
*/
main (int argc, char *argv[])
{
	Widget       toplevel, dialog;
	XtAppContext app;
	extern void  exit(int);
	Arg          args[5];
	int          n = 0;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);   

	XtSetArg (args[n], XmNfileSearchProc, do_search); n++;
	dialog = XmCreateFileSelectionDialog (toplevel, "Files", args, n);
	XtSetSensitive (XtNameToWidget (dialog, "Help"), False);
	
	/* if user presses OK button, call new_file_cb() */
	XtAddCallback (dialog, XmNokCallback, new_file_cb, NULL);
	
	/* if user presses Cancel button, exit program */
	XtAddCallback (dialog, XmNcancelCallback, (void (*)()) exit, NULL);
	XtManageChild (dialog);
	XtAppMainLoop (app);
}

/* a new file was selected -- check to see if it's readable and not
** a directory. If it's not readable, report an error. If it's a
** directory, scan it just as though the user had typed it in the mask
** Text field and selected "Search".
*/
void new_file_cb (Widget widget, XtPointer client_data, XtPointer call_data)
{
	char *file;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
	
	/* get the string typed in the text field in char * format */
	if (!(file = (char *) XmStringUnparse (cbs->value, XmFONTLIST_DEFAULT_TAG, 
					       XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL)))
		return;
		
	if (*file != '/') {
		/* if it's not a directory, determine the full pathname 
		** of the selection by concatenating it to the "dir" part
		*/
		
		char *dir, *newfile; 
		
		if (dir = XmStringUnparse (cbs->dir, XmFONTLIST_DEFAULT_TAG, 
					     XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL)) {
			newfile = XtMalloc (strlen (dir) + 1 + strlen (file) + 1);
			
			sprintf (newfile, "%s/%s", dir, file);

			XtFree (file);
			XtFree (dir);

			file = newfile;
		}
	}
	
	switch (is_writable (file)) {
		case 1: puts (file); /* or do anything you want */ break;
		case 0: {
			/* a directory was selected, scan it */
			
			XmString str = XmStringCreateLocalized (file);
			
			XmFileSelectionDoSearch (widget, str);
			XmStringFree (str);
			break;
		}
		
		case -1:
			/* a system error on this file */
			perror (file);
	}
	
	XtFree (file);
}

/* do_search() -- scan a directory and report only those files that
** are writable. Here, we let the shell expand the (possible)
** wildcards and return a directory listing by using popen().
** A *real* application should -not- do this; it should use the
** system's directory routines: opendir(), readdir() and closedir(). 
*/
void do_search (Widget    widget, /* file selection box widget */
                XtPointer search_data,
		XtPointer call_data)
{
	char          *mask, buf[BUFSIZ], *p;
	XmString       names[256]; /* maximum of 256 files in dir */
	int            i = 0;
	FILE          *pp, *popen();
	
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) search_data;
	
	if (!(mask = (char *) XmStringUnparse (cbs->mask, XmFONTLIST_DEFAULT_TAG, 
					       XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL)))
		return; /* can't do anything */
		
	sprintf (buf, "/bin/ls %s", mask);
	XtFree (mask);
	
	/* let the shell read the directory and expand the filenames */
	
	if (!(pp = popen (buf, "r")))
		return;
		
	/* read output from popen() -- this will be the list of files */
	while (fgets (buf, sizeof buf, pp)) {
		if (p = index (buf, '\n'))
			*p = 0;
			
		/* only list files that are writable and not directories */
		if (is_writable (buf) == 1 &&  (names[i] = XmStringCreateLocalized (buf))) i++; 
	}
	
	pclose (pp); 
	
	if (i) {
		XtVaSetValues (widget,
			       XmNfileListItems, names,
			       XmNfileListItemCount, i,
			       XmNdirSpec, names[0],
			       XmNlistUpdated, True,
			       NULL);        

		while (i > 0)
			XmStringFree (names[--i]);
	} else
		XtVaSetValues (widget,
			       XmNfileListItems, NULL, 
			       XmNfileListItemCount, 0,
			       XmNlistUpdated, True,
			       NULL);
}
