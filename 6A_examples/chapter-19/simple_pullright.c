/* simple_pullright.c -- demonstrate how to make a pullright menu
** using simple menu creation routines. Create a main window that
** contains a DrawingArea widget that displays a popup menu when the
** user presses the third mouse button.
*/

#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>

main (int argc, char *argv[])
{
	XmString     line, square, circle, weight, exit, exit_acc;
	XmString     w_one, w_two, w_four, w_eight;
	Widget       toplevel, main_w, drawing_a, cascade, popup_menu, pullright;
	void         popup_cb(Widget, XtPointer, XtPointer);
	void         set_width(Widget, XtPointer, XtPointer);
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
	XtSetArg (args[0], XmNscrollingPolicy, XmAUTOMATIC); n++;
	main_w = XmCreateMainWindow (toplevel, "main_w", args, n);
	
	/* Create a DrawingArea -- no actual drawing will be done. */
	n = 0;
	XtSetArg (args[n], XmNwidth, 500); n++;
	XtSetArg (args[n], XmNheight, 500); n++;
	drawing_a = XmCreateDrawingArea (main_w, "drawing_a", args, n);
	XtManageChild (drawing_a);
	
	line = XmStringCreateLocalized ("Line");
	square = XmStringCreateLocalized ("Square");
	circle = XmStringCreateLocalized ("Circle");
	weight = XmStringCreateLocalized ("Line Width");
	exit = XmStringCreateLocalized ("Exit");
	exit_acc = XmStringCreateLocalized ("Ctrl+C");
	popup_menu = XmVaCreateSimplePopupMenu (drawing_a, "popup", popup_cb,
	                                        XmNpopupEnabled, XmPOPUP_AUTOMATIC,
						XmVaPUSHBUTTON, line, 'L', NULL, NULL,
						XmVaPUSHBUTTON, square, 'S', NULL, NULL,
						XmVaPUSHBUTTON, circle, 'C', NULL, NULL,
						XmVaCASCADEBUTTON, weight, 'W',
						XmVaSEPARATOR,
						XmVaPUSHBUTTON, exit, 'x', "Ctrl<Key>c", exit_acc,
						NULL);
	XmStringFree (line);
	XmStringFree (square);
	XmStringFree (circle);
	XmStringFree (weight);
	XmStringFree (exit);
	
	/* create pullright for "Line Width" button -- this is the 4th item! */
	w_one = XmStringCreateLocalized ("1");
	w_two = XmStringCreateLocalized ("2");
	w_four = XmStringCreateLocalized ("4");
	w_eight = XmStringCreateLocalized ("8");
	pullright = XmVaCreateSimplePulldownMenu (popup_menu, "pullright", 3 /* menu item offset */, set_width,
						  XmVaPUSHBUTTON, w_one, '1', NULL, NULL,
						  XmVaPUSHBUTTON, w_two, '2', NULL, NULL,
						  XmVaPUSHBUTTON, w_four, '4', NULL, NULL,
						  XmVaPUSHBUTTON, w_eight, '8', NULL, NULL,
						  NULL);
	XmStringFree (w_one);
	XmStringFree (w_two);
	XmStringFree (w_four);
	XmStringFree (w_eight);
	
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
/* popup_cb() -- invoked when the user selects an item in the popup menu */
void popup_cb (Widget menu_item, XtPointer client_data, XtPointer call_data)
{
	int item_no = (int) client_data;
	if (item_no == 4) /* Exit was selected -- exit */
		exit (0);
	/* Otherwise, just print the selection */
	puts (XtName (menu_item));
}
	
/* set_width() -- called when items in the Line Width pullright menu
** are selected.
*/
void set_width (Widget menu_item, XtPointer client_data, XtPointer call_data)
{
	int item_no = (int) client_data;
	printf ("Line weight = %d\n", 1 << item_no);
}
