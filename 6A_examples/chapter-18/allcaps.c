/* allcaps.c -- demonstrate the XmNmodifyVerifyCallback for
** Text widgets by using one to convert all typed input to
** capital letters.
*/

#include <Xm/Text.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <ctype.h>

void allcaps(Widget, XtPointer, XtPointer);

main (int argc, char *argv[])
{
	Widget       toplevel, text_w, rowcol, label_w;
	XtAppContext app;
	Arg          args[2]; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	XtSetArg (args[0], XmNorientation, XmHORIZONTAL);
	rowcol = XmCreateRowColumn (toplevel, "rowcol", args, 1);

	label_w = XmCreateLabelGadget (rowcol, "Enter Text:", NULL, 0);
	XtManageChild (label_w);
	text_w = XmCreateText (rowcol, "text_w", NULL, 0);
	XtManageChild (text_w);

	XtAddCallback (text_w, XmNmodifyVerifyCallback, allcaps, NULL);

	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* allcaps() -- convert inserted text to capital letters. */
void allcaps (Widget text_w, XtPointer client_data, XtPointer call_data)
{
	int len;
	XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct *) call_data;

	if (cbs->text->ptr == NULL) 
		return;
		
	/* convert all input to upper-case if necessary */
	for (len = 0; len < cbs->text->length; len++)
		if (islower (cbs->text->ptr[len]))
			cbs->text->ptr[len] = toupper (cbs->text->ptr[len]);
}
