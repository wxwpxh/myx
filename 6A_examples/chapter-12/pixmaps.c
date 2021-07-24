/* pixmaps.c -- Demonstrate simple label gadgets in a row column
** Each command line argument represents a bitmap filename. Try
** to load the corresponding pixmap and store in a RowColumn.
*/

#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>

main (int argc, char *argv[])
{
	XtAppContext app;
	Pixel        fg, bg;
	Widget       toplevel, rowcol, pb;
	Arg          args[6];
	int          n;
	int          int_sqrt(int);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	if (argc < 2) {
		puts ("Specify bitmap filenames.");
		exit (1);
	}

	/* create a RowColumn that has an equal number of rows and
	** columns based on the number of pixmaps it is going to
	** display (this value is in "argc").
	*/
	
	n = 0;
	XtSetArg (args[n], XmNpacking, XmPACK_COLUMN); n++;
	XtSetArg (args[n], XmNnumColumns,  int_sqrt (argc)); n++;
	rowcol = XmCreateRowColumn (toplevel, "rowcol", args, n);
	
	/* Get the foreground and background colors of the rowcol to make
	** all the pixmaps appear using a consistent color.
	*/
	
	XtVaGetValues (rowcol,  XmNforeground, &fg,  XmNbackground, &bg,  NULL);
	
	while (*++argv) {
		Pixmap pixmap = XmGetPixmap (XtScreen (rowcol), *argv, fg, bg);
		
		if (pixmap == XmUNSPECIFIED_PIXMAP)
			printf ("Couldn't load %s\n", *argv);
		else {
			n = 0;
			XtSetArg (args[n], XmNlabelType, XmPIXMAP); n++;
			XtSetArg (args[n], XmNlabelPixmap, pixmap); n++;
			pb = XmCreateLabelGadget (rowcol, *argv, args, n);
			XtManageChild (pb);
		}
	}
	
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* get the integer square root of n -- used to determine the number
** of rows and columns of pixmaps to use in the RowColumn widget.
*/
int int_sqrt (register int n)
{
	register int i, s = 0, t;

	for (i = 15; i >= 0; i--) {
		t = (s | (1 << i));
		
		if (t * t <= n)
			s = t;
	}
	
	return s;
}
