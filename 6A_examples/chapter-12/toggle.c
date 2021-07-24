/* toggle.c -- demonstrate a simple toggle button. 
*/

#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>

void toggled(Widget widget, XtPointer client_data, XtPointer call_data)
{
	XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;
	
	printf ("%s: %s\n", XtName (widget),
		state->set == XmSET? "on" : state->set == XmOFF ? "off" : "indeterminate");
}

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, toggle;
	XtAppContext app;
	Pixmap       on, off, unknown;
	Pixel        fg, bg;
	Arg          args[6];
	int          n;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0,  &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	n = 0;
	XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
	rowcol = XmCreateRowColumn (toplevel, "rowcol", args, n);
	XtVaGetValues (rowcol, XmNforeground, &fg, XmNbackground, &bg, NULL);
	on = XmGetPixmap (XtScreen (rowcol), "switch_on.xbm", fg, bg);
	off = XmGetPixmap (XtScreen (rowcol), "switch_off.xbm", fg, bg);
	unknown = XmGetPixmap (XtScreen (rowcol), "switch_unknown.xbm", fg, bg);
	
	if (on == XmUNSPECIFIED_PIXMAP  || off == XmUNSPECIFIED_PIXMAP || unknown == XmUNSPECIFIED_PIXMAP) {
		puts ("Couldn't load pixmaps");
		exit (1);
	}

	n = 0;
	XtSetArg (args[n], XmNtoggleMode, XmTOGGLE_INDETERMINATE); n++;
	XtSetArg (args[n], XmNlabelType, XmPIXMAP); n++;
	XtSetArg (args[n], XmNlabelPixmap, off); n++;
	XtSetArg (args[n], XmNselectPixmap, on); n++;
	XtSetArg (args[n], XmNindeterminatePixmap, unknown); n++;

	toggle = XmCreateToggleButton (rowcol, "toggle", args, n);
	XtAddCallback (toggle, XmNvalueChangedCallback, toggled, NULL);
	XtManageChild (toggle);

	toggle = XmCreateToggleButton (rowcol, "toggle",args, n);
	XtAddCallback (toggle, XmNvalueChangedCallback, toggled, NULL);
	XtManageChild (toggle);

	toggle = XmCreateToggleButton (rowcol, "toggle",args, n);
	XtAddCallback (toggle, XmNvalueChangedCallback, toggled, NULL);
	XtManageChild (toggle);

	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
