/* free_hand.c -- simple drawing program that does freehand
** drawing.  We use translations to do all the event handling
** for us rather than using the drawing area's XmNinputCallback.
*/
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>

/* Global variables */
GC gc;
Pixmap pixmap;
Dimension width, height;

main (int argc, char *argv[])
{
	Widget       toplevel, main_w, drawing_a, pb;
	XtAppContext app;
	XGCValues    gcv;
        void         draw (Widget, XEvent *, String *, Cardinal *) ;
	void         redraw (Widget, XtPointer, XtPointer) ;
	void         clear_it (Widget, XtPointer, XtPointer) ;
	XtActionsRec actions;
	Arg          args[10];
	int          n;
	String       translations = /* for the DrawingArea widget */
		                    /* ManagerGadget* functions are necessary for DrawingArea widgets
				    ** that steal away button events from the normal translation tables.
				    */
				    "<Btn1Down>:   draw(down)   ManagerGadgetArm()\n\
				     <Btn1Up>:     draw(up)     ManagerGadgetActivate()\n\
				     <Btn1Motion>: draw(motion) ManagerGadgetButtonMotion()";
		
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0,  &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);
					 
	/* Create a MainWindow to contain the drawing area */
	n = 0;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
	main_w = XmCreateMainWindow (toplevel, "main_w", args, n);

	/* Add the "draw" action/function used by the translation table */
	actions.string = "draw";
	actions.proc = draw;
	XtAppAddActions (app, &actions, 1);
	
	/* Create a DrawingArea widget.  Make it 5 inches wide by 6 inches tall.
	** Don't let it resize so the Clear Button doesn't force a resize.
	*/

	n = 0;
	XtSetArg (args[n], XmNtranslations, XtParseTranslationTable (translations)); n++;
	XtSetArg (args[n], XmNunitType, Xm1000TH_INCHES); n++;
	XtSetArg (args[n], XmNwidth, 5000); n++; /* 5 inches */
	XtSetArg (args[n], XmNheight, 6000); n++; /* 6 inches */
	XtSetArg (args[n], XmNresizePolicy, XmRESIZE_NONE); n++; /* remain this a fixed size */ 
	drawing_a = XmCreateDrawingArea (main_w, "drawing_a", args, n);
	
	/* When scrolled, the drawing area will get expose events */
	XtAddCallback (drawing_a, XmNexposeCallback, redraw, NULL); 
	
	/* convert drawing area back to pixels to get its width and height */ 
	XtVaSetValues (drawing_a, XmNunitType, XmPIXELS, NULL);
	XtVaGetValues (drawing_a, XmNwidth, &width, XmNheight, &height, NULL);
	
	/* create a pixmap the same size as the drawing area. */
	pixmap = XCreatePixmap (XtDisplay (drawing_a), RootWindowOfScreen (XtScreen (drawing_a)), width, height, 
				DefaultDepthOfScreen (XtScreen (drawing_a)));
	
	/* Create a GC for drawing (callback).  Used a lot -- make global */
	gcv.foreground = WhitePixelOfScreen (XtScreen (drawing_a));
	gc = XCreateGC (XtDisplay (drawing_a), RootWindowOfScreen (XtScreen (drawing_a)), GCForeground, &gcv);
	
	/* clear pixmap with white */
	XFillRectangle (XtDisplay (drawing_a), pixmap, gc, 0, 0, width, height);
	
	/* drawing is now drawn into with "black"; change the gc */
	XSetForeground (XtDisplay (drawing_a), gc, BlackPixelOfScreen (XtScreen (drawing_a)));
	pb = XmCreatePushButtonGadget (drawing_a, "Clear",NULL, 0);
	XtManageChild (pb);
	
	/* Pushing the clear button calls clear_it() */
	XtAddCallback (pb, XmNactivateCallback, clear_it, (XtPointer) drawing_a);

	XtManageChild (drawing_a);
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* Action procedure to respond to any of the events from the
** translation table declared in main().  This function is called
** in response to Button1 Down, Up and Motion events.  Basically,
** we're just doing a freehand draw -- not lines or anything.
*/
void draw (Widget widget, XEvent *event, String *args, Cardinal *num_args)
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
	
	/* freehand is really a bunch of line segments; save this point */
	
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
	XFillRectangle (XtDisplay (drawing_a), pixmap, gc, 0, 0, width, height);
	
	/* drawing is now done using black; change the gc */
	XSetForeground (XtDisplay (drawing_a), gc, BlackPixelOfScreen (XtScreen (drawing_a)));
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
