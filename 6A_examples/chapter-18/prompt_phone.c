/* prompt_phone.c -- a complex problem for XmNmodifyVerifyCallback.
** Prompt for a phone number by filtering digits only from input.
** Don't allow paste operations and handle backspacing.
*/

#include <Xm/Text.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <ctype.h>

void check_phone(Widget, XtPointer, XtPointer);

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
	
	label_w = XmCreateLabelGadget (rowcol, "Phone Number:", NULL, 0);
	XtManageChild (label_w);
	
	text_w = XmCreateText (rowcol, "text_w", NULL, 0);
	XtManageChild (text_w);
	
	XtAddCallback (text_w, XmNmodifyVerifyCallback, check_phone, NULL);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
/* check_phone() -- handle phone number input. */
void check_phone (Widget text_w, XtPointer client_data, XtPointer  call_data)
{
	char c;
	int len = XmTextGetLastPosition(text_w);
	XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct *) call_data;
	
	/* no backspacing or typing in the middle of string */
	if (cbs->currInsert < len) {
		cbs->doit = False;
		return;
	}
	
	if (cbs->text->length == 0) { /* backspace */
		if (cbs->startPos == 3 || cbs->startPos == 7)
			cbs->startPos--;
		/* delete the hyphen too */
		return;
	}

	if (cbs->text->length > 1) { /* don't allow clipboard copies */
		cbs->doit = False;
		return;
	}
	
	/* don't allow non-digits or let the input exceed 12 chars */
	if (!isdigit (c = cbs->text->ptr[0]) || len >= 12)
		cbs->doit = False;
	else if (len == 2 || len == 6) {
		cbs->text->ptr = XtRealloc (cbs->text->ptr, 2);
		cbs->text->length = 2;
		cbs->text->ptr[0] = c;
		cbs->text->ptr[1] = '-';
	}
}
	
