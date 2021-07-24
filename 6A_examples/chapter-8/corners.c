/* corners.c -- demonstrate widget layout management for a
** BulletinBoard widget. There are four widgets each labelled
** top-left, top-right, bottom-left and bottom-right. Their
** positions in the bulletin board correspond to their names.
** Only when the widget is resized does the geometry management
** kick in and position the children in their correct locations.
*/
#include <Xm/BulletinB.h>
#include <Xm/PushB.h>

char *corners[] = { "Top Left", "Top Right", "Bottom Left", "Bottom Right" };
void  resize(Widget, XEvent *, String *, Cardinal *);

main (int argc, char *argv[])
{
	Widget       toplevel, bboard, button;
	XtAppContext app;
	XtActionsRec rec; 
	int          i;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	/* Initialize toolkit and create toplevel shell */
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	/* Create your standard BulletinBoard widget */
	bboard = XmCreateBulletinBoard (toplevel, "bboard", NULL, 0);
	
	/* Set up a translation table that captures "Resize" events 
	** (also called ConfigureNotify or Configure events). If the
	** event is generated, call the function resize().
	*/
	
	rec.string = "resize";
	rec.proc   = resize;
	
	XtAppAddActions (app, &rec, 1);
	XtOverrideTranslations (bboard,  XtParseTranslationTable ("<Configure>: resize()"));
	
	/* Create children of the dialog -- a PushButton in each corner. */
	for (i = 0; i < XtNumber (corners); i++) {
		button = XmCreatePushButton (bboard, corners[i], NULL, 0);
		XtManageChild (button);
	}
	
	XtManageChild (bboard);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* resize(), the routine that is automatically called by Xt upon the
** delivery of a Configure event. This happens whenever the widget
** gets resized.
*/
void resize ( Widget w, /* The widget (BulletinBoard) that got resized */
	      XEvent *event, /* The event struct associated with the event */
	      String args[], /* unused */
	      Cardinal *num_args) /* unused */
{
	WidgetList       children;
	Dimension        w_width, w_height;
	short            margin_w, margin_h;
	XConfigureEvent *cevent = (XConfigureEvent *) event;
	int              width = cevent->width;
	int              height = cevent->height;
	
	/* get handle to BulletinBoard's children and marginal spacing */
	XtVaGetValues (w, XmNchildren, &children, 
			  XmNmarginWidth, &margin_w, 
			  XmNmarginHeight, &margin_h, 
			  NULL);

	/* place the top left widget */
	XtVaSetValues (children[0],  XmNx, margin_w, XmNy, margin_h, NULL);
	
	/* top right */
	XtVaGetValues (children[1], XmNwidth, &w_width, NULL);
	XtVaSetValues (children[1],  XmNx, width - margin_w - w_width, XmNy, margin_h, NULL);
	
	/* bottom left */
	XtVaGetValues (children[2], XmNheight, &w_height, NULL);
	XtVaSetValues (children[2],  XmNx, margin_w, XmNy, height - margin_h - w_height, NULL);
	
	/* bottom right */
	XtVaGetValues (children[3],  XmNheight, &w_height, XmNwidth, &w_width, NULL);
	XtVaSetValues (children[3],  XmNx, width - margin_w - w_width,
				     XmNy, height - margin_h - w_height,
				     NULL);
}
