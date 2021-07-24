/* simple_option.c -- demonstrate how to use a simple option menu.
** Display a drawing area. The user selects the drawing style from
** the option menu.
*/

#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/ScrolledW.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>

main (int argc, char *argv[])
{
	XmString     draw_shape, line, square, circle;
	Widget       toplevel, main_w, rc, sw, drawing_a, option_menu, pb;
	void         option_cb(Widget, XtPointer, XtPointer);
	void         exit(int);
	XtAppContext app;
	Arg          args[4];
	int          n;

	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	/* Create a MainWindow widget that contains a RowColumn
	** widget as its work window.
	*/
	main_w = XmCreateMainWindow (toplevel, "main_w", NULL, 0);
	rc = XmCreateRowColumn (main_w, "rowcol", NULL, 0);
	
	/* Inside RowColumn is the Exit pushbutton, the option menu and the
	** scrolled window that contains the drawing area.
	*/
	pb = XmCreatePushButton (rc, "Exit", NULL, 0);
	XtAddCallback (pb, XmNactivateCallback, (void (*)()) exit, NULL);
	XtManageChild (pb);
	
	draw_shape = XmStringCreateLocalized ("Draw Mode:");
	line = XmStringCreateLocalized ("Line");
	square = XmStringCreateLocalized ("Square");
	circle = XmStringCreateLocalized ("Circle");
	option_menu = XmVaCreateSimpleOptionMenu (rc, 
			     "option_menu", draw_shape, 'D', 0 /*initial menu selection*/, option_cb,
			     XmVaPUSHBUTTON, line, 'L', NULL, NULL,
			     XmVaPUSHBUTTON, square, 'S', NULL, NULL,
			     XmVaPUSHBUTTON, circle, 'C', NULL, NULL,
			     NULL);
	XmStringFree (line);
	XmStringFree (square);
	XmStringFree (circle);
	XmStringFree (draw_shape);
	XtManageChild (option_menu);
	
	/* Create a DrawingArea inside a ScrolledWindow */
	n = 0;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
	sw = XmCreateScrolledWindow (rc, "sw", args, n);
	
	n = 0;
	XtSetArg (args[n], XmNwidth, 500); n++;
	XtSetArg (args[n], XmNheight, 500); n++;
	drawing_a = XmCreateDrawingArea (sw, "drawing_area", args, n);
	XtManageChild (drawing_a);

	XtManageChild (sw);
	XtManageChild (rc);
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* option_cb() -- invoked when the user selects an item in the
** option menu
*/
void option_cb (Widget menu_item, XtPointer client_data, XtPointer call_data)
{
	int item_no = (int) client_data;
	puts (XtName (menu_item));
}
