/* simple_popup.c -- demonstrate how to use a simple popup menu.
** Create a main window that contains a DrawingArea widget, which
** displays a popup menu when the user presses the third mouse button.
*/

#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>

main (int argc, char *argv[])
{
	XmString     line, square, circle, exit, exit_acc;
	Widget       toplevel, main_w, drawing_a, popup_menu;
	void         popup_cb(Widget, XtPointer, XtPointer);
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
	XtManageChild (drawing_a);
	
	line = XmStringCreateLocalized ("Line");
	square = XmStringCreateLocalized ("Square");
	circle = XmStringCreateLocalized ("Circle");
	exit = XmStringCreateLocalized ("Exit");
	exit_acc = XmStringCreateLocalized ("Ctrl+C");
	popup_menu = XmVaCreateSimplePopupMenu (drawing_a, "popup", popup_cb,
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
	
