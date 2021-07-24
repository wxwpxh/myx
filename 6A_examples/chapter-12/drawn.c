/* drawn.c -- demonstrate the DrawnButton widget by drawing a
** common X logo into its window. This is hardly much different
** from a PushButton widget, but the DrawnButton isn't much
** different, except for a couple more callback routines...
*/

#include <Xm/DrawnB.h>
#include <Xm/BulletinB.h>

Pixmap pixmap = (Pixmap) 0 ;

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, bb, button;
	Pixel        fg, bg;
	Dimension    ht, st;
	void         my_callback(Widget, XtPointer, XtPointer);
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	bb = XmCreateBulletinBoard (toplevel, "bb", NULL, 0);
	XtVaGetValues (bb, XmNforeground, &fg, XmNbackground, &bg, NULL);
	pixmap = XmGetPixmap (XtScreen (bb), "xlogo64", fg, bg);
	button = XmCreateDrawnButton (bb, "button", NULL, 0);
	XtManageChild (button);
	
	XtVaGetValues (button, XmNhighlightThickness, &ht, XmNshadowThickness, &st, NULL);
	XtVaSetValues (button, XmNwidth, 2 * ht + 2 * st + 64, XmNheight, 2 * ht + 2 * st + 64, NULL);
	
	XtAddCallback (button, XmNactivateCallback, my_callback, NULL);
	XtAddCallback (button, XmNexposeCallback, my_callback, NULL);
	XtAddCallback (button, XmNresizeCallback, my_callback, NULL);
	
	XtManageChild (bb);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
void my_callback (Widget w, XtPointer client_data, XtPointer call_data)
{
	XmDrawnButtonCallbackStruct *cbs = (XmDrawnButtonCallbackStruct *) call_data;

	printf("%x\n", cbs->event);
	
	if (cbs->reason == XmCR_ACTIVATE)
		printf ("%s: pushed %d times\n", XtName(w), cbs->click_count);
	else if (cbs->reason == XmCR_EXPOSE) {
		Dimension ht, st;
	
		XtVaGetValues (w, XmNhighlightThickness, &ht, XmNshadowThickness, &st, NULL);
		XtVaSetValues (w, XmNwidth, 2 * ht + 2 * st + 64, XmNheight, 2 * ht + 2 * st + 64, NULL);
	
		XCopyArea (XtDisplay (w), pixmap, XtWindow (w),
		XDefaultGCOfScreen (XtScreen (w)), 0, 0, 64, 64, ht + st, ht + st);
	}
	else /* XmCR_RESIZE */
		puts ("Resize");
}
