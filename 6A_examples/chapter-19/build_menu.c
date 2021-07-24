/* build_menu.c -- Demonstrate the BuildMenu() routine and
** how it can be used to build pulldown -and- pullright menus.
** Menus are defined by declaring an array of MenuItem structures.
*/

#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/CascadeBG.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

typedef struct _menu_item 
{
	char              *label;          /* the label for the item */
	WidgetClass       *class;          /* pushbutton, label, separator... */
	char               mnemonic;       /* mnemonic; NULL if none */
	char              *accelerator;    /* accelerator; NULL if none */
	char              *accel_text;     /* to be converted to compound string */
	void             (*callback)();    /* routine to call; NULL if none */
	XtPointer          callback_data;  /* client_data for callback() */
	struct _menu_item *subitems;       /* pullright menu items, if not NULL */
} MenuItem;

/* Pulldown menus are built from cascade buttons, so this function
** also includes pullright menus. Create the menu, the cascade button
** that owns the menu, and then the submenu items.
*/
Widget BuildMenu (Widget parent, int menu_type, char *menu_title, char menu_mnemonic, 
                  Boolean tear_off, MenuItem *items)
{
	Widget   menu, cascade, widget;
	int      i;
	XmString str;
	Arg      args[8];
	int      n;

	if (menu_type == XmMENU_PULLDOWN)	
		menu = XmCreatePulldownMenu (parent, "_pulldown", NULL, 0);
	else {
		n = 0;
		XtSetArg (args[n], XmNpopupEnabled, XmPOPUP_AUTOMATIC_RECURSIVE); n++;
		menu = XmCreatePopupMenu (parent, "_popup", args, n);
	}
	
	if (tear_off) 
		XtVaSetValues (menu, XmNtearOffModel, XmTEAR_OFF_ENABLED, NULL);
		
	if (menu_type == XmMENU_PULLDOWN) {
		str = XmStringCreateLocalized (menu_title);
		n = 0;
		XtSetArg (args[n], XmNsubMenuId, menu); n++;
		XtSetArg (args[n], XmNlabelString, str); n++;
		XtSetArg (args[n], XmNmnemonic, menu_mnemonic); n++;
		cascade = XmCreateCascadeButtonGadget (parent, menu_title, args, n);
		XtManageChild (cascade);
		XmStringFree (str);
	}

	/* Now add the menu items */
	for (i = 0; items[i].label != NULL; i++) {
		/* If subitems exist, create the pull-right menu by calling this
		** function recursively. Since the function returns a cascade
		** button, the widget returned is used.. 
		*/
		
		if (items[i].subitems)
			widget = BuildMenu (menu, XmMENU_PULLDOWN, items[i].label, 
                                                items[i].mnemonic, tear_off, items[i].subitems);
		else
			widget = XtVaCreateManagedWidget (items[i].label, *items[i].class, menu, NULL);

		/* Whether the item is a real item or a cascade button with a
		** menu, it can still have a mnemonic.
		*/
		if (items[i].mnemonic)
			XtVaSetValues (widget, XmNmnemonic, items[i].mnemonic, NULL);
			
		/* any item can have an accelerator, except cascade menus. But,
		** we don't worry about that; we know better in our declarations.
		*/
		if (items[i].accelerator) {
			str = XmStringCreateLocalized (items[i].accel_text);
			
			XtVaSetValues (widget, XmNaccelerator, items[i].accelerator, XmNacceleratorText, 
				       str, NULL);
			XmStringFree (str);
		}
		
		if (items[i].callback) {
			String resource;
			
			if (XmIsToggleButton(widget) || XmIsToggleButtonGadget(widget))
				resource = XmNvalueChangedCallback;
			else
				resource = XmNactivateCallback;
				
			XtAddCallback(widget, resource, items[i].callback,(XtPointer) items[i].callback_data);
		}
	}

	return (menu_type == XmMENU_PULLDOWN) ? cascade : menu ;
}

/* callback functions for menu items declared later... */
void set_weight (Widget widget, XtPointer client_data, XtPointer call_data)
{
	int weight = (int) client_data;
	printf ("Setting line weight to %d\n", weight);
}

void set_color (Widget widget, XtPointer client_data, XtPointer call_data)
{
	char *color = (char *) client_data;
	printf ("Setting color to %s\n", color);
}

void set_dot_dash (Widget widget, XtPointer client_data, XtPointer call_data)
{
	int dot_or_dash = (int) client_data;
	printf ("Setting line style to %s\n", dot_or_dash? "dot" : "dash");
}

MenuItem weight_menu[] = {
	{ "1", &xmPushButtonGadgetClass, '1', NULL, NULL, set_weight, (XtPointer) 1, (MenuItem *) NULL },
	{ "2", &xmPushButtonGadgetClass, '2', NULL, NULL, set_weight, (XtPointer) 2, (MenuItem *) NULL },
	{ "3", &xmPushButtonGadgetClass, '3', NULL, NULL, set_weight, (XtPointer) 3, (MenuItem *) NULL },
	{ "4", &xmPushButtonGadgetClass, '4', NULL, NULL, set_weight, (XtPointer) 4, (MenuItem *) NULL },
	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

MenuItem color_menu[] = {
	{ "Cyan", &xmPushButtonGadgetClass, 'C', "Alt<Key>C", "Alt+C", set_color,
		(XtPointer) "cyan", (MenuItem *) NULL },
	{ "Yellow", &xmPushButtonGadgetClass, 'Y', "Alt<Key>Y", "Alt+Y", set_color,
		(XtPointer) "yellow", (MenuItem *) NULL },
	{ "Magenta", &xmPushButtonGadgetClass, 'M', "Alt<Key>M", "Alt+M", set_color,
		(XtPointer) "magenta", (MenuItem *) NULL },
	{ "Black", &xmPushButtonGadgetClass, 'B', "Alt<Key>B", "Alt+B", set_color,
		(XtPointer) "black", (MenuItem *) NULL }, 
	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

MenuItem style_menu[] = {
	{ "Dash", &xmPushButtonGadgetClass, 'D', NULL, NULL, set_dot_dash, (XtPointer) 0, (MenuItem *) NULL },
	{ "Dot", &xmPushButtonGadgetClass, 'o', NULL, NULL, set_dot_dash, (XtPointer) 1, (MenuItem *) NULL },
	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

MenuItem drawing_menus[] = {
	{ "Line Weight", &xmCascadeButtonGadgetClass, 'W', NULL, NULL, 0, 0, weight_menu },
	{ "Line Color", &xmCascadeButtonGadgetClass, 'C', NULL, NULL, 0, 0, color_menu },
	{ "Line Style", &xmCascadeButtonGadgetClass, 'S', NULL, NULL, 0, 0, style_menu },
	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

main (int argc, char *argv[])
{
	Widget       toplevel, main_w, menubar, drawing_a; 
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
	menubar = XmCreateMenuBar (main_w, "menubar", NULL, 0);
	BuildMenu (menubar, XmMENU_PULLDOWN, "Lines", 'L', True, drawing_menus);
	XtManageChild (menubar);
	
	/* Create a DrawingArea -- no actual drawing will be done. */
	n = 0;
	XtSetArg (args[n], XmNwidth, 500); n++;
	XtSetArg (args[n], XmNheight, 500); n++;
	drawing_a = XmCreateDrawingArea (main_w, "drawing_a", args, n);
	XtManageChild (drawing_a);
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
