/* color_draw.c -- simple drawing program using predefined colors.*/

#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>

GC     gc     = (GC) 0 ;
Pixmap pixmap = (Pixmap) 0 ;

/* dimensions of drawing area (pixmap) */
Dimension width, height;

String colors[] = {"Black", "Red", "Green", "Blue", "White", "Navy", "Orange", "Yellow", "Pink", 
		   "Magenta", "Cyan", "Brown", "Grey", "LimeGreen", "Turquoise", "Violet", "Wheat", 
		   "Purple"};

main (int argc, char *argv[])
{
	Widget         toplevel, main_w, sw, rc, form, drawing_a, pb;
	XtAppContext   app;
	XGCValues      gcv;
	Arg            args[12];
	void           draw (Widget, XEvent *, String *,  Cardinal *);
	void           redraw(Widget, XtPointer, XtPointer) ;
	void           clear_it(Widget, XtPointer, XtPointer) ;
	void           set_color(Widget, XtPointer, XtPointer) ;
	void           exit (int) ;
	int            i, n;
	XtActionsRec   actions;
	XtTranslations parsed_xa;
	String         translations =  /* for the DrawingArea widget */  "<Btn1Down>:   draw(down)\n\
								          <Btn1Up>:     draw(up)  \n\
								          <Btn1Motion>: draw(motion)";


	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);
					 
	 /* Create a MainWindow to contain the drawing area */
	 main_w = XmCreateForm (toplevel, "main_w", NULL, 0);

	/* Create a GC for drawing (callback).  Used a lot -- make global */
	gcv.foreground = WhitePixelOfScreen (XtScreen (main_w));
	gc = XCreateGC (XtDisplay (main_w), RootWindowOfScreen (XtScreen (main_w)), GCForeground, &gcv);
	
	/* Create a 3-column array of color tiles */
	n = 0;
	XtSetArg (args[n], XmNpacking, XmPACK_COLUMN); n++;
	XtSetArg (args[n], XmNnumColumns, 3); n++;
	XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
	rc = XmCreateRowColumn (main_w, "rc", args, n);
	
	for (i = 0; i < XtNumber(colors); i++) {
		/* Create a single tile (pixmap) for each color */
		pixmap = XCreatePixmap (XtDisplay (rc), RootWindowOfScreen (XtScreen (rc)), 16, 16, 
					DefaultDepthOfScreen (XtScreen (rc)));
					
		set_color (rc, colors[i], NULL); /* set the gc's color according to name */
		
		XFillRectangle (XtDisplay (main_w), pixmap, gc, 0, 0, 16, 16);

		n = 0;
		XtSetArg (args[n], XmNlabelType, XmPIXMAP); n++;
		XtSetArg (args[n], XmNlabelPixmap, pixmap); n++;
		pb = XmCreatePushButton (rc, colors[i], args, n);
		
		/* callback for this pushbutton sets the current color */
		XtAddCallback (pb, XmNactivateCallback, set_color, (XtPointer) colors[i]);
		XtManageChild (pb);
	}
	
	XtManageChild (rc);

	n = 0;
	XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
	XtSetArg (args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
	XtSetArg (args[n], XmNtopWidget, rc); n++;
	pb = XmCreatePushButton (main_w, "Quit",  args, n);
	XtAddCallback (pb, XmNactivateCallback, (void (*)()) exit, NULL);
	XtManageChild (pb);
	
	/* Clear button -- wait till DrawingArea is created so we can use 
	** it to pass as client data. 
	*/
	n = 0;
	XtSetArg (args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
	XtSetArg (args[n], XmNleftWidget, pb); n++;
	XtSetArg (args[n], XmNtopAttachment, XmATTACH_WIDGET); n++; 
	XtSetArg (args[n], XmNtopWidget, rc); n++;
	pb = XmCreatePushButton (main_w, "Clear", args, n);
	XtManageChild (pb);
	
	n = 0;
	XtSetArg (args[n], XmNwidth, 300); n++;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
	XtSetArg (args[n], XmNscrollBarDisplayPolicy, XmAS_NEEDED); n++;
	XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	XtSetArg (args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
	XtSetArg (args[n], XmNleftWidget, rc); n++;
	XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
	sw = XmCreateScrolledWindow (main_w, "scrolled_win",  args, n);
	
	/* Add the "draw" action/function used by the translation table 
	** parsed by the translations resource below.
	*/
	actions.string = "draw";
	actions.proc = draw;
	XtAppAddActions (app, &actions, 1);
	
	/* Create a DrawingArea widget.  Make it 5 inches wide by 6 inches tall.
	** Don't let it resize so the Clear Button doesn't force a resize. 
	*/
	
	parsed_xa = XtParseTranslationTable (translations);
	n = 0;
	XtSetArg (args[n], XmNunitType, Xm1000TH_INCHES); n++;
	XtSetArg (args[n], XmNwidth, 5000); n++; /* 5 inches */
	XtSetArg (args[n], XmNheight, 6000); n++; /* 6 inches */
	XtSetArg (args[n], XmNresizePolicy, XmNONE); n++;  /* remain this a fixed size */
	XtSetArg (args[n], XmNtranslations, parsed_xa); n++; 
	drawing_a = XmCreateDrawingArea (sw, "drawing_a", args, n);

	/* When scrolled, the drawing area will get expose events */
	XtAddCallback (drawing_a, XmNexposeCallback, redraw, NULL);
	XtManageChild (drawing_a);

	/* Pushing the clear button clears the drawing area widget */
	XtAddCallback (pb, XmNactivateCallback, clear_it, (XtPointer) drawing_a);

	/* convert drawing area back to pixels to get its width and height */
	XtVaSetValues (drawing_a, XmNunitType, XmPIXELS, NULL);
	XtVaGetValues (drawing_a, XmNwidth, &width, XmNheight, &height, NULL);

	/* create a pixmap the same size as the drawing area. */
	pixmap = XCreatePixmap (XtDisplay (drawing_a), RootWindowOfScreen (XtScreen (drawing_a)), width, height, 
                                DefaultDepthOfScreen (XtScreen (drawing_a)));

	/* clear pixmap with white */
	set_color (drawing_a, "White", NULL);
	XFillRectangle (XtDisplay (drawing_a), pixmap, gc, 0, 0, width, height);

	XtManageChild (sw);
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* Action procedure to respond to any of the events from the
** translation table declared in main().  This function is called
** in response to Button1 Down, Up and Motion events.  Basically,
** we're just doing a freehand draw -- not lines or anything.
*/
void draw (Widget widget, XEvent *event, String *args,  Cardinal *num_args)
{
	static Position x, y;
	XButtonEvent *bevent = (XButtonEvent *) event;
	
	if (*num_args != 1)
		XtError ("Wrong number of args!");
		
	if (strcmp (args[0], "down")) {
		/* if it's not "down", it must either be "up" or "motion"
		** draw full line from anchor point to new point.
		*/
		XDrawLine (bevent->display, bevent->window, gc, x, y, bevent->x, bevent->y);
		XDrawLine (bevent->display, pixmap, gc, x, y, bevent->x, bevent->y);
	}
	
	/* freehand is really a bunch of line segements; save this point */
	x = bevent->x;
	y = bevent->y;
}

/* Clear the window by clearing the pixmap and calling XCopyArea() */
void clear_it (Widget pb, XtPointer client_data, XtPointer call_data)
{
	Widget drawing_a = (Widget) client_data;
	XmPushButtonCallbackStruct *cbs = (XmPushButtonCallbackStruct *) call_data; 
	
	/* clear pixmap with white */
	XSetForeground (XtDisplay (drawing_a), gc, WhitePixelOfScreen (XtScreen (drawing_a)));
	/* this clears the pixmap */
	XFillRectangle (XtDisplay (drawing_a), pixmap, gc, 0, 0, width, height);
	/* drawing is now done using black; change the gc */
	XSetForeground (XtDisplay (drawing_a), gc, BlackPixelOfScreen (XtScreen (drawing_a)));
	/* render the newly cleared pixmap onto the window */
	XCopyArea (cbs->event->xbutton.display, pixmap, XtWindow (drawing_a), gc, 0, 0, width, height, 0, 0);
}

/* redraw is called whenever all or portions of the drawing area is
** exposed.  This includes newly exposed portions of the widget resulting
** from the user's interaction with the scrollbars.
*/
void redraw (Widget drawing_a, XtPointer client_data, XtPointer call_data)
{
	XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) call_data;
	XCopyArea (cbs->event->xexpose.display, pixmap, cbs->window, gc, 0, 0, width, height, 0, 0);
}

/* callback routine for when any of the color tiles are pressed.
** This general function may also be used to set the global gc's
** color directly.  Just provide a widget and a color name.
*/
void set_color (Widget widget, XtPointer client_data, XtPointer call_data)
{
	String   color = (String) client_data;
	Display *dpy = XtDisplay (widget);
	Colormap cmap = DefaultColormapOfScreen (XtScreen (widget));
	XColor   col, unused;
	
	if (!XAllocNamedColor (dpy, cmap, color, &col, &unused)) {
		char buf[32];
		sprintf (buf, "Can't alloc %s", color);
		XtWarning (buf);
		return;
	}
	
	XSetForeground (dpy, gc, col.pixel);
}
