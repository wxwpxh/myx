/* simple_notebook - create a Notebook with four pages
*/

#include <Xm/Xm.h>
#include <Xm/Notebook.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>

/* Arbitrary data for the first "page" */
char *page1_labels[] =
{
	"Introduction",
	"",
	"The Motif Programming Model",
	"Overview of the Motif Toolkit",
	"The Main Window",
	"Introduction to Dialogs",
	(char *) 0,
};

/* Arbitrary data for the second "page" */
char *page2_labels[] =
{
	"Selection Dialogs",
	"Custom Dialogs",
	"Manager Widgets",
	"The Container and IconGadget Widgets",
	"Scrolled Windows and ScrollBars",
	(char *) 0
};

/* Arbitrary data for the third "page" */
char *page3_labels[] =
{
	"The DrawingArea",
	"Labels and Buttons",
	"The List Widget",
	"The ComboBox Widget",
	"The SpinBox and SimpleSpinBox Widgets",
	(char *) 0
};

/* Arbitrary data for the fourth "page" */
char *page4_labels[] =
{
	"The Scale Widget",
	"The Notebook Widget",
	"Text Widgets",
	"Menus",
	(char *) 0
};

/* A pointer to all the "page" data */
char **pages[] =
{
	page1_labels,
	page2_labels,
	page3_labels,
	page4_labels,
	(char **) 0
};

void main (int argc, char **argv)
{
	Widget       toplevel, notebook, page, label, tab;
	XtAppContext app;
	Arg          args[4];
	XmString     xms;
	int          i, j, n;
	char         buffer[32];

	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	/* Create the Notebook */
	notebook = XmCreateNotebook (toplevel, "notebook", NULL, 0);

	/* Create the "pages" */
	for (i = 0 ; pages[i] != (char **) 0 ; i++) {
		page = XmCreateRowColumn (notebook, "page", NULL, 0);

		for (j = 0 ; pages[i][j] != (char *) 0 ; j++) {
			xms = XmStringGenerate (pages[i][j], XmFONTLIST_DEFAULT_TAG, XmCHARSET_TEXT, NULL);

			n = 0;
			XtSetArg(args[n], XmNlabelString, xms); n++;
			label = XmCreateLabel (page, "label", args, n);
			XtManageChild (label);
			XmStringFree (xms) ;
		}

		/* An even page is a Major Tab */
		/* And an odd page is a Minor Tab */
		n = 0;
		if ((i % 2) == 0) { 
			XtSetArg (args[n], XmNnotebookChildType, XmMAJOR_TAB); n++;
		}
		else { 
			XtSetArg (args[n], XmNnotebookChildType, XmMINOR_TAB); n++;
		}
		(void) sprintf (buffer, "%s %d", ((i % 2) == 0) ? "Major" : "Minor", i);
		tab = XmCreatePushButton (notebook, buffer, args, n);
		XtManageChild (tab);
	
		XtManageChild (page) ;
	}

	XtManageChild (notebook) ;
	XtRealizeWidget (toplevel) ;
	XtAppMainLoop(app) ;
}
