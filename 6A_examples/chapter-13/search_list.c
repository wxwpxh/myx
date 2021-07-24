/* search_list.c -- search for items in a List and select them
*/

#include <stdio.h>
#include <Xm/List.h>
#include <Xm/LabelG.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/PanedW.h>
#include <Xm/TextF.h>

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, list_w, text_w, label_w;
	XtAppContext app;
	Arg          args[5];
	int          n = 0;
	XmString     label;
	void         add_item(Widget, XtPointer, XtPointer);
	void         search_item(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);
	
	rowcol = XmCreatePanedWindow (toplevel, "rowcol", NULL, 0);
	
	label = XmStringCreateLocalized ("List:");
	n = 0;
	XtSetArg (args[n], XmNlabelString, label); n++;
	label_w = XmCreateLabel (rowcol, "list_lable", args, n);
	XmStringFree (label);
	XtManageChild (label_w);
	
	n = 0;
	XtSetArg (args[n], XmNvisibleItemCount, 10); n++;
	XtSetArg (args[n], XmNselectionPolicy, XmEXTENDED_SELECT); n++;
	list_w = XmCreateScrolledList (rowcol, "scrolled_list", args, n);
	XtManageChild (list_w);
	
	label = XmStringCreateLocalized ("Add:");
	n = 0;
	XtSetArg (args[n], XmNlabelString, label); n++;
	label_w = XmCreateLabel (rowcol, "add_label", args, n);
	XmStringFree (label);
	XtManageChild (label_w);
	
	n = 0;
	XtSetArg (args[n], XmNcolumns, 25); n++;
	text_w = XmCreateTextField (rowcol, "add_text",args, n);
	XtAddCallback (text_w, XmNactivateCallback, add_item, (XtPointer) list_w);
	XtManageChild (text_w);
	
	label = XmStringCreateLocalized ("Search:");
	n = 0;
	XtSetArg (args[n], XmNlabelString, label); n++;
	label_w = XmCreateLabel (rowcol, "search_label", args, n);
	XmStringFree (label);
	XtManageChild (label_w);
	
	n = 0;
	XtSetArg (args[n], XmNcolumns, 25); n++;
	text_w = XmCreateTextField (rowcol, "search_text", args, n);
	XtAddCallback (text_w, XmNactivateCallback, search_item, (XtPointer) list_w);
	XtManageChild (text_w);
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
/* Add item to the list in alphabetical order.  Perform binary
** search to find the correct location for the new item position.
** This is the callback routine for the Add: TextField widget.
*/
void add_item (Widget text_w, XtPointer client_data, XtPointer call_data)
{
	Widget   list_w = (Widget) client_data;
	char    *text, *newtext = XmTextFieldGetString (text_w);
	XmString str, *strlist;
	int      u_bound, l_bound = 0;
	
	if (!newtext || !*newtext) {
		/* non-null strings must be entered */
		XtFree (newtext);
		return;
	}
	
	XtVaGetValues (list_w, XmNitemCount, &u_bound, XmNitems, &strlist, NULL);
	u_bound--;
	
	/* perform binary search */
	while (u_bound >= l_bound) {
		int i = l_bound + (u_bound - l_bound)/2;
	
		if (!(text = (char *) XmStringUnparse (strlist[i], XmFONTLIST_DEFAULT_TAG,
						    XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL)))
			break;

		if (strcmp (text, newtext) > 0)
			u_bound = i-1; /* newtext comes before item */
		else
			l_bound = i+1; /* newtext comes after item */

		XtFree (text);
	}
	
	str = XmStringCreateLocalized (newtext);
	XtFree (newtext);
	
	/* positions indexes start at 1, so increment accordingly */
	XmListAddItemUnselected (list_w, str, l_bound+1);
	XmStringFree (str);
	XmTextFieldSetString (text_w, "");
}
	
/* find the item in the list that matches the specified pattern */
void search_item (Widget text_w, XtPointer client_data, XtPointer call_data)
{
	Widget    list_w = (Widget) client_data;
	char     *exp, *text, *newtext = XmTextFieldGetString (text_w);
	XmString *strlist, *selectlist = NULL;
	int       matched, cnt, j = 0;
#ifndef   SYSV
	extern char *re_comp(char *);
#endif /* SYSV */
	
	if (!newtext || !*newtext) {
		/* non-null strings must be entered */
		XtFree (newtext);
		return;
	}
	
	/* compile expression into pattern matching library */
#ifdef    SYSV
	if (!(exp = (char *) regcmp (newtext, NULL))) {
		printf ("Error with regcmp(%s)\n", newtext);
		XtFree (newtext);
		return;
	}
#else  /* SYSV */
	if (exp = re_comp (newtext)) {
		printf ("Error with re_comp(%s): %s\n", newtext, exp);
		XtFree (newtext);
		return;
	}
#endif /* SYSV */
	
	/* get all the items in the list ... we're going to search each one */
	XtVaGetValues (list_w, XmNitemCount, &cnt, XmNitems, &strlist, NULL);
	
	while (cnt--) {
		/* convert item to C string */
		if (!(text = (char *) XmStringUnparse (strlist[cnt], XmFONTLIST_DEFAULT_TAG, 
						       XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL)))
			break;
	
		/* do pattern match against search string */
#ifdef SYSV
		/* returns NULL if match failed */
		matched = regex (exp, text, NULL) != NULL;
#else /* BSD */
		/* -1 on error, 0 if no-match, 1 if match */
		matched = re_exec (text) > 0;
#endif /* SYSV */
		if (matched) {
			selectlist = (XmString *) XtRealloc ((char *) selectlist, (j+1) * (sizeof (XmString *)));
			selectlist[j++] = XmStringCopy (strlist[cnt]);
		}

		XtFree (text);
	}

#ifdef    SYSV
	free (exp);
	/* this must be freed for regcmp() */
#endif /* SYSV */
	
	XtFree (newtext);
	
	/* set the actual selected items to be those that matched */
	XtVaSetValues (list_w, XmNselectedItems, selectlist, XmNselectedItemCount, j, NULL);
	
	while (j--)
		XmStringFree (selectlist[j]);
	
	XmTextFieldSetString (text_w, "");
}
