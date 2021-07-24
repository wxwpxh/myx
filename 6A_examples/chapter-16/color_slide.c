/* color_slide.c -- Use scale widgets to display the different
** colors of a colormap.
*/

#include <Xm/LabelG.h>
#include <Xm/Scale.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>

Widget colorwindow; /* the window the displays a solid color */
XColor color; /* the color in the colorwindow */

Widget create_scale (Widget parent, char *name, int mask)
{
	Arg      args[8];
	int      n = 0;
	Widget   scale;
	XrmValue from;
	XrmValue to;
	XmString xms = XmStringCreateLocalized(name);
	void     new_value(Widget, XtPointer, XtPointer);
	
	to.addr = NULL;
	from.addr = name;
	from.size = strlen(name) + 1;
	XtConvertAndStore (parent, XmRString, &from, XmRPixel, &to);
	
	XtSetArg (args[n], XmNforeground, (*(Pixel *) to.addr)); n++;
	XtSetArg (args[n], XmNtitleString, xms); n++;
	XtSetArg (args[n], XmNshowValue, True); n++;
	XtSetArg (args[n], XmNmaximum, 255); n++;
	XtSetArg (args[n], XmNscaleMultiple, 5); n++;

	scale = XmCreateScale (parent, name, args, n);

	XmStringFree (xms);

	XtAddCallback (scale, XmNdragCallback, new_value, (XtPointer) mask);
	XtAddCallback (scale, XmNvalueChangedCallback, new_value, (XtPointer) mask);;
	XtManageChild (scale);
	
	return scale;
}

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, rc, scale;
	XtAppContext app;
	Arg          args[8];
	int          n;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	if (DefaultDepthOfScreen (XtScreen (toplevel)) < 2) {
		puts ("You must be using a color screen.");
		exit (1);
	}
	
	color.flags = DoRed | DoGreen | DoBlue;
	
	/* initialize first color */
	XAllocColor (XtDisplay (toplevel), DefaultColormapOfScreen (XtScreen (toplevel)), &color);
	
	rowcol = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);
	
	/* create a canvas for the color display */
	n = 0;
	XtSetArg (args[n], XmNheight, 100); n++;
	XtSetArg (args[n], XmNbackground, color.pixel); n++;
	colorwindow = XmCreateDrawingArea (rowcol, "colorwindow", args, n);
	XtManageChild (colorwindow);

	/* create another RowColumn under the 1st */
	n = 0;
	XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
	rc = XmCreateRowColumn (rowcol, "rc", args, n);

	/* create the scale widgets */
	scale = create_scale (rc, "Red", DoRed);
	scale = create_scale (rc, "Green", DoGreen);
	scale = create_scale (rc, "Blue", DoBlue);

	XtManageChild (rc);
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

void new_value (Widget scale_w, XtPointer client_data, XtPointer call_data)
{
	int                    rgb  = (int) client_data;
	XmScaleCallbackStruct *cbs  = (XmScaleCallbackStruct *) call_data;
	Colormap               cmap = DefaultColormapOfScreen (XtScreen (scale_w));

	switch (rgb) {
		case DoRed   : color.red   = (cbs->value << 8); break;
		case DoGreen : color.green = (cbs->value << 8); break;
		case DoBlue  : color.blue  = (cbs->value << 8);
	}
	
	/* reuse the same color again and again */
	XFreeColors (XtDisplay (scale_w), cmap, &color.pixel, 1, 0);
	
	if (!XAllocColor (XtDisplay (scale_w), cmap, &color)) {
		puts ("Couldn't XAllocColor!"); exit(1);
	}
	
	XtVaSetValues (colorwindow, XmNbackground, color.pixel, NULL);
}
