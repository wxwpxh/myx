/* search_text.c -- demonstrate how to position a cursor at a
** particular location. The position is determined by a pattern
** match search.
*/

#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <X11/Xos.h> 

/* for the index() function */
Widget text_w, search_w, text_output;

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol_v, rowcol_h, label_w;
	XtAppContext app;
	int          i, n;
	void         search_text(Widget, XtPointer, XtPointer);
	Arg          args[10];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos",  NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);

	rowcol_v = XmCreateRowColumn (toplevel, "rowcol_v",  NULL, 0);
	XtSetArg (args[0], XmNorientation, XmHORIZONTAL); 
	rowcol_h = XmCreateRowColumn (rowcol_v, "rowcol_h",  args, 1);
	label_w = XmCreateLabelGadget (rowcol_h, "Search Pattern:", NULL, 0);
	XtManageChild (label_w);
	search_w = XmCreateTextField (rowcol_h, "search_text", NULL, 0);
	XtManageChild (search_w);
	XtManageChild (rowcol_h);
	
	n = 0;
	XtSetArg (args[n], XmNeditable, False); n++;
	XtSetArg (args[n], XmNcursorPositionVisible, False); n++;
	XtSetArg (args[n], XmNshadowThickness, 0); n++;
	XtSetArg (args[n], XmNhighlightThickness, 0); n++;
	text_output = XmCreateText (rowcol_v, "text_output", args, n);
	XtManageChild (text_output);

	n = 0;
	XtSetArg (args[n], XmNrows, 10); n++;
	XtSetArg (args[n], XmNcolumns, 80); n++;
	XtSetArg (args[n], XmNeditMode,  XmMULTI_LINE_EDIT); n++;
	XtSetArg (args[n], XmNscrollHorizontal,  False); n++;
	XtSetArg (args[n], XmNwordWrap,  True); n++;
	text_w = XmCreateScrolledText (rowcol_v, "text_w", args, n);
	XtManageChild (text_w);
	
	XtAddCallback (search_w, XmNactivateCallback, search_text, NULL);
	
	XtManageChild (rowcol_v);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* search_text() -- called when the user activates the TextField. */
void search_text (Widget widget, XtPointer client_data, XtPointer call_data)
{
	char          *search_pat, *p, *string, buf[32];
	XmTextPosition pos;
	int            len;
	Boolean        found = False;
	
	/* get the text that is about to be searched */
	if (!(string = XmTextGetString (text_w)) || !*string) {
		XmTextSetString (text_output, "No text to search.");
		XtFree (string); /* may have been ""; free it */
		return;
	}
	
	/* get the pattern we're going to search for in the text. */
	if (!(search_pat = XmTextGetString (search_w)) || !*search_pat) {
		XmTextSetString (text_output, "Specify a search pattern.");
		XtFree (string); /* this we know is a string; free it */
		XtFree (search_pat); /* this may be "", XtFree() checks.. */
		return;
	}
	
	len = strlen (search_pat);

	/* start searching at current cursor position + 1 to find
	** the -next- occurrance of string.  we may be sitting on it.
	*/
	pos = XmTextGetCursorPosition (text_w);
	
	for (p = &string[pos+1]; p = index (p, *search_pat); p++)
		if (!strncmp (p, search_pat, len)) {
			found = True;
			break;
		}
		
	if (!found) { /* didn't find pattern? */
		/* search from beginning till we've passed "pos" */
		for (p = string; (p = index (p, *search_pat)) && p - string <= pos; p++)
			if (!strncmp (p, search_pat, len)) {
				found = True;
				break;
			}
	}
	
	if (!found)
		XmTextSetString (text_output, "Pattern not found.");
	else {
		pos = (XmTextPosition)(p - string);
		sprintf (buf, "Pattern found at position %ld.", pos);
		XmTextSetString (text_output, buf);
		XmTextSetInsertionPosition (text_w, pos);
	}
	
	XtFree (string);
	XtFree (search_pat);
}
