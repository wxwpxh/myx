/* password.c -- prompt for a password. All input looks like
** a series of *'s.  Store the actual data typed by the user in
** an internal variable.  Don't allow paste operations.  Handle
** backspacing by deleting all text from insertion point to the
** end of text.
*/

#include <Xm/TextF.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <ctype.h>

void check_passwd(Widget, XtPointer, XtPointer);
char *passwd = (char *) 0 ; /* store user-typed passwd here. */

main (int argc, char *argv[])
{
	Widget       toplevel, text_w, label_w, rowcol;
	XtAppContext app;
	Arg          args[2];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);

	XtSetArg (args[0], XmNorientation, XmHORIZONTAL);
	rowcol = XmCreateRowColumn (toplevel, "rowcol", args, 1);
	
	label_w = XmCreateLabelGadget (rowcol, "Password:", NULL, 0);
	XtManageChild (label_w);
	
	text_w = XmCreateTextField (rowcol, "text_w", NULL, 0);
	XtManageChild (text_w);
	
	XtAddCallback(text_w, XmNmodifyVerifyCallback, check_passwd, NULL);
	XtAddCallback(text_w, XmNactivateCallback, check_passwd, NULL);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* check_passwd() -- handle the input of a password. */
void check_passwd (Widget text_w, XtPointer client_data, XtPointer call_data)
{
	char *new;
	int   len;
	XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct *) call_data;
	
	if (cbs->reason == XmCR_ACTIVATE) {
		printf ("Password: %s\n", passwd);
		return;
	}
	
	if (cbs->startPos < cbs->currInsert) { /* backspace */
		cbs->endPos = strlen (passwd); /* delete from here to end */
		passwd[cbs->startPos] = 0;     /* backspace--terminate */
		return;
	}
	
	if (cbs->text->length > 1) {
		cbs->doit = False; /* don't allow "paste" operations */
		return;            /* make the user *type* the password! */
	}
	
	new = XtMalloc (cbs->endPos + 2); /* new char + NULL terminator */
	if (passwd) {
		strcpy (new, passwd);
		XtFree (passwd);
	} else
	new[0] = NULL;
	
	passwd = new;
	strncat (passwd, cbs->text->ptr, cbs->text->length);
	passwd[cbs->endPos + cbs->text->length] = 0;
	
	for (len = 0; len < cbs->text->length; len++)
		cbs->text->ptr[len] = '*';
}
