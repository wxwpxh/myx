/* parse_string.c: converts the /etc/passwd file into
** a multi-column list format
*/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/List.h>

/* construct an array of compound strings 
** from loading a file using colon as the field separator.
**
** A more generic routine would pass the field separator in.
*/
XmString *load_file (Widget list, char *file, int *count)
{
	XmParseMapping map[2];
	FILE          *fptr ;
	char           buffer[256];
	Arg            args[8];
	char          *cptr;
	XmString       tab;
	XmString      *xms_array = (XmString *) 0;
	int            xms_count = 0;
	int            n;

	*count = 0;

	if ((fptr = fopen (file, "r")) == (FILE *) 0) {
		return NULL;
	}

	/* Map colons to tabs */
	n = 0;
	tab = XmStringComponentCreate (XmSTRING_COMPONENT_TAB, 0, NULL);
	XtSetArg (args[n], XmNpattern, ":"); n++;
	XtSetArg (args[n], XmNpatternType, XmCHARSET_TEXT); n++;
	XtSetArg (args[n], XmNsubstitute, tab); n++;
	XtSetArg (args[n], XmNincludeStatus, XmINSERT); n++;
	map[0] = XmParseMappingCreate (args, n);

	/* Throw away newlines by terminating the parse for this line */
	n = 0;
	XtSetArg (args[n], XmNpattern, "\n"); n++;
	XtSetArg (args[n], XmNpatternType, XmCHARSET_TEXT); n++;
	XtSetArg (args[n], XmNincludeStatus, XmTERMINATE); n++;
	map[1] = XmParseMappingCreate (args, n);

	while ((cptr = fgets (buffer, 255, fptr)) != (char *) 0) {
		xms_array = (XmString *) XtRealloc ((char *) xms_array, (xms_count + 1) * sizeof (XmString));

		xms_array[xms_count] = XmStringParseText (cptr, 
							  NULL,
							  NULL,
							  XmCHARSET_TEXT,
							  map,
							  2,
							  NULL);

		xms_count++;
	}

	(void) fclose(fptr);

	XmParseMappingFree (map[0]);
	XmParseMappingFree (map[1]);

	*count = xms_count;

	return xms_array;
}

main (int argc, char *argv[])
{
	Widget        toplevel, list;
	XtAppContext  app;
	Arg           args[8];
	XmTabList     tlist = NULL;
	XmRenderTable rtable;
	XmRendition   rendition;
	XmString     *data  = (XmString *) 0;
	int           lines = 0;
	int           n, i;

        XtSetLanguageProc (NULL, NULL, NULL);
        toplevel = XtVaOpenApplication (&app, "Demo", NULL, 0, &argc, argv, NULL,
                                        sessionShellWidgetClass, NULL);

	n = 0;
	XtSetArg (args[n], XmNvisibleItemCount, 10); n++;	
	/* For TabList separation calculations */
	XtSetArg (args[n], XmNunitType, XmINCHES); n++;	
	list = XmCreateScrolledList (toplevel, "list", args, n);
	XtManageChild (list);

	/* Load the data from file as an array of compound strings */
	data = load_file (list, ((argc > 1) ? argv[1] : "/etc/passwd"), &lines);

	XtVaSetValues (list, XmNitems, data, XmNitemCount, lines, NULL);

	/* Use the toolkit to propose a tab list for the items */
	tlist = XmStringTableProposeTablist (data, lines, list, (float) 0.1, XmRELATIVE);
	
	/* Create a render table for the List using the tab list */
	n = 0;
	XtSetArg (args[n], XmNtabList, tlist); n++;
	XtSetArg (args[n], XmNfontName, "-*-courier-*-r-*--*-120-*"); n++;
	XtSetArg (args[n], XmNfontType, XmFONT_IS_FONT); n++;
	rendition = XmRenditionCreate (list, XmFONTLIST_DEFAULT_TAG, args, n);
	rtable = XmRenderTableAddRenditions (NULL, &rendition, 1, XmMERGE_REPLACE);
	XtVaSetValues (list, XmNrenderTable, rtable, NULL);

	/* Free up the temporarily allocated memory */
	XmRenditionFree (rendition);
	XmRenderTableFree (rtable);
	XmTabListFree (tlist);

	for (i = 0 ; i < lines ; i++)
		XmStringFree (data[i]);

	XtFree ((char *) data);

	/* Display the interface */
	XtRealizeWidget (toplevel);
	XtAppMainLoop(app);
}
