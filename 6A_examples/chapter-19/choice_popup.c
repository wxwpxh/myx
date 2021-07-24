/* choice_popup.c -- demonstrate how to use a popup menu handler.
** Create a main window that contains a DrawingArea widget, which
** choses between two popup menus when the user presses the third 
** mouse button.
*/

#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>

Widget menuA, menuB;

main (int argc, char *argv[])
{
	XmString     line, square, circle, exit, exit_acc;
	XmString     red, green, blue;
	Widget       toplevel, main_w, drawing_a;
	void         popup_cb(Widget, XtPointer, XtPointer);
	void         choose_cb(Widget, XtPointer, XtPointer);
	XtAppContext app;
	Arg          args[4];
	int          n;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	
	/* Create a MainWindow widget that contains a DrawingArea in 
	** its work window. 
	*/
	n = 0;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
	main_w = XmCreateMainWindow (toplevel, "main_w", args, n);
	
	/* Create a DrawingArea -- no actual drawing will be done. */
	n = 0;
	XtSetArg (args[n], XmNwidth, 500); n++;
	XtSetArg (args[n], XmNheight, 500); n++;
	drawing_a = XmCreateDrawingArea (main_w, "drawing_a", args, n);
	/* Callback to choose which popup menu to display */
	XtAddCallback (drawing_a, XmNpopupHandlerCallback, choose_cb, NULL);
	XtManageChild (drawing_a);
	
	line = XmStringCreateLocalized ("Line");
	square = XmStringCreateLocalized ("Square");
	circle = XmStringCreateLocalized ("Circle");
	exit = XmStringCreateLocalized ("Exit");
	exit_acc = XmStringCreateLocalized ("Ctrl+C");
	menuA = XmVaCreateSimplePopupMenu (drawing_a, "menuA", popup_cb,
	                                   XmNpopupEnabled, XmPOPUP_AUTOMATIC,
					   XmVaPUSHBUTTON, line, 'L', NULL, NULL,
					   XmVaPUSHBUTTON, square, 'S', NULL, NULL,
					   XmVaPUSHBUTTON, circle, 'C', NULL, NULL,
					   XmVaSEPARATOR,
					   XmVaPUSHBUTTON, exit, 'x', "Ctrl<Key>c", exit_acc,
					   NULL);
	XmStringFree (line);
	XmStringFree (square);
	XmStringFree (circle);
	XmStringFree (exit);
	XmStringFree (exit_acc);
	
	red = XmStringCreateLocalized ("Red");
	green = XmStringCreateLocalized ("Green");
	blue = XmStringCreateLocalized ("Blue");
	
	menuB = XmVaCreateSimplePopupMenu (drawing_a, "menuB", popup_cb,
	                                   XmNpopupEnabled, XmPOPUP_AUTOMATIC,
					   XmVaPUSHBUTTON, red, 'R', NULL, NULL,
					   XmVaPUSHBUTTON, green, 'G', NULL, NULL,
					   XmVaPUSHBUTTON, blue, 'B', NULL, NULL,
					   NULL);
	XmStringFree (red);
	XmStringFree (green);
	XmStringFree (blue);
	
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
/* popup_cb() -- invoked when the user selects an item in the popup menu */
void popup_cb (Widget menu_item, XtPointer client_data, XtPointer call_data)
{
	int item_no = (int) client_data;
	
	if (item_no == 3) /* Exit was selected -- exit */
		exit (0);
	
	/* Otherwise, just print the selection */
	puts (XtName (menu_item));
}
	
/* choose_cb() -- invoked when the user requests a popup menu */
void choose_cb (Widget menu_item, XtPointer client_data, XtPointer call_data)
{
	XmPopupHandlerCallbackStruct *cbs = (XmPopupHandlerCallbackStruct *) call_data ;
	XButtonPressedEvent          *bp  = (XButtonPressedEvent *) cbs->event ;
	
	if ((bp->x % 2) == 0) {
		cbs->menuToPost = menuA ;
	}
	else {
		cbs->menuToPost = menuB ;
	}
}
